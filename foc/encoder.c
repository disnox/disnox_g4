#include "encoder.h"
#include "main.h"
#include "spi.h"
#include "util.h"
#include "user_config.h"
tEncoder Encoder;

void Encoder_init(void)
{
		// Init
	Encoder.count_in_cpr_ = 0;
	Encoder.shadow_count_ = 0;
	Encoder.pos_estimate_counts_ = 0.0f;
	Encoder.vel_estimate_counts_ = 0.0f;
	Encoder.pos_cpr_counts_ = 0.0f;
	
	Encoder.pos_estimate_ = 0.0f;
	Encoder.vel_estimate_ = 0.0f;
	Encoder.pos_cpr_ = 0.0f;
	
	Encoder.phase_ = 0.0f;
	Encoder.interpolation_ = 0.0f;
	
	int encoder_pll_bandwidth = 2000;
	Encoder.pll_kp_ = 2.0f * encoder_pll_bandwidth;  				// basic conversion to discrete time
  Encoder.pll_ki_ = 0.25f * (Encoder.pll_kp_ * Encoder.pll_kp_); 	// Critically damped
}

uint32_t ENCODER_read_raw(void)
{

	uint8_t data_t[2];
	uint8_t data_r[4];
	uint32_t count;
	data_t[0] = 0x83;
	data_t[1] = 0xff;

	cs_down;
	HAL_SPI_Transmit(&hspi3,&data_t[0],1,1000);
	//HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[3],1,1000);
	HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[0],1,1000);
	HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[1],1,1000);
	HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[2],1,1000);

	cs_up;

	data_r[1] >>= 2;
	data_r[2] >>= 4;
	count = data_r[0] << 10 | data_r[1] << 4 | data_r[2] ;

	return count;
}


void ENCODER_sample(void)
{
	int32_t delta_enc = 0;
    Encoder.raw = ENCODER_read_raw();
	
	if(UsrConfig.calib_valid){
		// offset compensation
		int off_1 = UsrConfig.offset_lut[Encoder.raw>>11];
		int off_2 = UsrConfig.offset_lut[((Encoder.raw>>11)+1)%128];
		int off_interp = off_1 + ((off_2 - off_1) * (Encoder.raw - ((Encoder.raw>>11)<<11)) >> 11);		// Interpolate between lookup table entries
		int cnt = Encoder.raw - off_interp;                                     						// Correct for nonlinearity with lookup table from calibration
		if(cnt > ENCODER_CPR){
			cnt -= ENCODER_CPR;
		}else if(cnt < 0){
			cnt += ENCODER_CPR;
		}
		Encoder.cnt = cnt;
	}else{
		Encoder.cnt = Encoder.raw;
	}
	
	delta_enc = Encoder.cnt - Encoder.count_in_cpr_;
	delta_enc = mod(delta_enc, ENCODER_CPR);
	if (delta_enc > ENCODER_CPR_DIV_2) {
		delta_enc -= ENCODER_CPR;
	}
	
	Encoder.shadow_count_ += delta_enc;
    Encoder.count_in_cpr_ += delta_enc;
    Encoder.count_in_cpr_ = mod(Encoder.count_in_cpr_, ENCODER_CPR);
	
	Encoder.count_in_cpr_ = Encoder.cnt;
	
	//// run pll (for now pll is in units of encoder counts)
    // Predict current pos  预测当前位置
    Encoder.pos_estimate_counts_ += DT * Encoder.vel_estimate_counts_;
    Encoder.pos_cpr_counts_      += DT * Encoder.vel_estimate_counts_;
    // discrete phase detector
    float delta_pos_counts = (float)(Encoder.shadow_count_ - (int32_t)floor(Encoder.pos_estimate_counts_));
    float delta_pos_cpr_counts = (float)(Encoder.count_in_cpr_ - (int32_t)floor(Encoder.pos_cpr_counts_));
    delta_pos_cpr_counts = wrap_pm(delta_pos_cpr_counts, ENCODER_CPR_DIV_2);
    // pll feedback
    Encoder.pos_estimate_counts_ += DT * Encoder.pll_kp_ * delta_pos_counts;
    Encoder.pos_cpr_counts_ += DT * Encoder.pll_kp_ * delta_pos_cpr_counts;
    Encoder.pos_cpr_counts_ = fmodf_pos(Encoder.pos_cpr_counts_, (float)(ENCODER_CPR));
    Encoder.vel_estimate_counts_ += DT * Encoder.pll_ki_ * delta_pos_cpr_counts;
    bool snap_to_zero_vel = false;
    if (ABS(Encoder.vel_estimate_counts_) < 0.5f * DT * Encoder.pll_ki_) {
        Encoder.vel_estimate_counts_ = 0.0f;  // align delta-sigma on zero to prevent jitter
        snap_to_zero_vel = true;
    }
	
	// Outputs from Encoder for Controller
    float pos_cpr_last = Encoder.pos_cpr_;
    Encoder.pos_estimate_ = Encoder.pos_estimate_counts_ / (float)ENCODER_CPR;
    Encoder.vel_estimate_ = Encoder.vel_estimate_counts_ / (float)ENCODER_CPR;
    Encoder.pos_cpr_= Encoder.pos_cpr_counts_ / (float)ENCODER_CPR;
    float delta_pos_cpr = wrap_pm(Encoder.pos_cpr_ - pos_cpr_last, 0.5f);

	//// run encoder count interpolation
    int32_t corrected_enc = Encoder.count_in_cpr_ - UsrConfig.encoder_offset;
    // if we are stopped, make sure we don't randomly drift
    if (snap_to_zero_vel) {
        Encoder.interpolation_ = 0.5f;
    // reset interpolation if encoder edge comes
    // TODO: This isn't correct. At high velocities the first phase in this count may very well not be at the edge.
    } else if (delta_enc > 0) {
        Encoder.interpolation_ = 0.0f;
    } else if (delta_enc < 0) {
        Encoder.interpolation_ = 1.0f;
    } else {
        // Interpolate (predict) between encoder counts using vel_estimate,
        Encoder.interpolation_ += DT * Encoder.vel_estimate_counts_;
        // don't allow interpolation indicated position outside of [enc, enc+1)
        if (Encoder.interpolation_ > 1.0f) Encoder.interpolation_ = 1.0f;
        if (Encoder.interpolation_ < 0.0f) Encoder.interpolation_ = 0.0f;
    }
    float interpolated_enc = corrected_enc + Encoder.interpolation_;
	
	//// compute electrical phase
    float elec_rad_per_enc = UsrConfig.motor_pole_pairs * M_2PI * (1.0f / (float)ENCODER_CPR);
    float ph = elec_rad_per_enc * interpolated_enc;
		Encoder.pos_abs_ =  Encoder.raw / 262144.0f * 360.0f;
    Encoder.phase_ = wrap_pm_pi(ph);
}



