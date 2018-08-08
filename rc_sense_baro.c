/*******************************************************************************
Scripts modified from Strawson Design Example Library
RenierSF1 2017
*******************************************************************************/

// http://strawsondesign.com/docs/librobotcontrol/group___barometer___b_m_p.html

// usefulincludes is a collection of common system includes for the lazy
// This is not necessary for roboticscape projects but here for convenience
#include <rc_usefulincludes.h> 
// main roboticscape API header
#include <roboticscape.h>

#define OVERSAMPLE  BMP_OVERSAMPLE_16
#define INTERNAL_FILTER	 BMP_FILTER_OFF

// function declarations
void on_pause_pressed();
void on_pause_released();



/*******************************************************************************
* int main() 
*******************************************************************************/
int main(){
	rc_mpu_data_t data; //struct to hold new data
	rc_bmp_data_t bmp_data; //struct to hold new data

	// always initialize cape library first
	/*
	if(rc_initialize()){
		fprintf(stderr,"ERROR: failed to initialize rc_initialize(), are you root?\n");
		return -1;
	}
	*/


		// do your own initialization here
	// done initializing so set state to RUNNING
	rc_set_state(RUNNING); 

	// use defaults for now, except also enable magnetometer.
	rc_mpu_config_t conf = rc_mpu_default_config();
	conf.enable_magnetometer=1;

	if(rc_mpu_initialize(&data, conf)){
		fprintf(stderr,"rc_mpu_initialize_failed\n");
		return -1;
	}

	if(rc_bmp_init(OVERSAMPLE, INTERNAL_FILTER)<0){
		fprintf(stderr,"ERROR: rc_bmp_init failed\n");
		return -1;
	}


	printf("time,a_x,a_y,a_z,");
	printf("omega_x,omega_y,omega_z,");
	printf("mag_x, mag_y, mag_z,");
	printf("temp,");
	printf("baro_temp,");
	printf("pressure,");
	printf("altitude");
	printf("\n");


	// Keep looping until state changes to EXITING
	while(rc_get_state()!=EXITING){
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		long int time = ((unsigned long) now.tv_sec)*1000000LL + now.tv_nsec / 1000;
		printf("%lu,", time);

		// handle other states
		if(rc_mpu_read_accel(&data)<0){
			printf("read accel data failed\n");
		}
		else
		{
			printf("%.2f,%.2f,%.2f,",	data.accel[0],\
											data.accel[1],\
											data.accel[2]);
		}

		if(rc_mpu_read_gyro(&data)<0){
			printf("read gyro data failed\n");
		}
		else
		{

			printf("%.1f,%.1f,%.1f,",	data.gyro[0],\
											data.gyro[1],\
											data.gyro[2]);
		}

		if(rc_mpu_read_mag(&data)<0){
			printf("read mag data failed\n");
		}
		else {
			printf("%.1f,%.1f,%.1f,",	data.mag[0],\
											data.mag[1],\
											data.mag[2]);
		}

		if(rc_mpu_read_temp(&data)<0){
			printf("read temp data failed\n");
		}
		else printf("%.1f,", data.temp);
		// always sleep at some point

		if(rc_bmp_read(&bmp_data)<0){
			fprintf(stderr,"\rERROR: Can't read Barometer");
			fflush(stdout);
			continue;
		}

		printf("%.2f,", bmp_data.temp_c);
		printf("%.2f,", bmp_data.pressure_pa);
		printf("%.2f\n", bmp_data.alt_m);
		
		
		fflush(stdout);
		rc_usleep(10000);
	}
	
	// exit cleanly
	//rc_power_off_barometer();
	rc_mpu_power_off();
	//rc_cleanup();
	return 0;
}


