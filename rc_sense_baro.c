/*******************************************************************************

*******************************************************************************/

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

void clrscr()
{
    system("@cls||clear");
}


/*******************************************************************************
* int main() 
*
* This template main function contains these critical components
* - call to rc_initialize() at the beginning
* - main while loop that checks for EXITING condition
* - rc_cleanup() at the end
*******************************************************************************/
int main(){
	rc_imu_data_t data; //struct to hold new data
	double temp, pressure, altitude;
	// always initialize cape library first
	if(rc_initialize()){
		fprintf(stderr,"ERROR: failed to initialize rc_initialize(), are you root?\n");
		return -1;
	}

	clrscr();

		// do your own initialization here
	printf("\nHello BeagleBone\n");
	// done initializing so set state to RUNNING
	rc_set_state(RUNNING); 

	// use defaults for now, except also enable magnetometer.
	rc_imu_config_t conf = rc_default_imu_config();
	conf.enable_magnetometer=1;

	if(rc_initialize_imu(&data, conf)){
		fprintf(stderr,"rc_initialize_imu_failed\n");
		return -1;
	}

	if(rc_initialize_barometer(OVERSAMPLE, INTERNAL_FILTER)<0){
	fprintf(stderr,"ERROR: rc_initialize_barometer failed\n");
	return -1;
	}


	printf("   Accel XYZ(m/s^2)  |");
	printf("   Gyro XYZ (deg/s)  |");
	printf("  Mag Field XYZ(uT)  |");
	printf(" Temp (C) |");
	printf("  Baro Temp (C) |");
	printf(" Pressure  |");
	printf(" Altitude |");
	printf("\n");



	// Keep looping until state changes to EXITING
	while(rc_get_state()!=EXITING){
		printf("\r");
		// handle other states
		if(rc_read_accel_data(&data)<0){
			printf("read accel data failed\n");
		}
		else
		{
			printf("%6.2f %6.2f %6.2f |",	data.accel[0],\
											data.accel[1],\
											data.accel[2]);
		}

		if(rc_read_gyro_data(&data)<0){
			printf("read gyro data failed\n");
		}
		else
		{

			printf("%6.1f %6.1f %6.1f |",	data.gyro[0],\
											data.gyro[1],\
											data.gyro[2]);
		}

		if(rc_read_mag_data(&data)<0){
			printf("read mag data failed\n");
		}
		else {
			printf("%6.1f %6.1f %6.1f |",	data.mag[0],\
											data.mag[1],\
											data.mag[2]);
		}

		if(rc_read_imu_temp(&data)<0){
			printf("read temp data failed\n");
		}
		else printf("   %4.1f   |", data.temp);
		// always sleep at some point

		if(rc_read_barometer()<0){
			fprintf(stderr,"\rERROR: Can't read Barometer");
			fflush(stdout);
			continue;
		}

		// if we got here, new data was read and is ready to be accessed.
		// these are very fast function calls and don't actually use i2c
		temp = rc_bmp_get_temperature();
		pressure = rc_bmp_get_pressure_pa();
		altitude = rc_bmp_get_altitude_m();

		printf("    %6.2fC     |", temp);
		printf("%7.2fkpa |", pressure/1000.0);
		printf("%8.2fm |", altitude);
		
		
		fflush(stdout);
		rc_usleep(100000);
	}
	
	// exit cleanly
	rc_power_off_barometer();
	rc_power_off_imu();
	rc_cleanup();
	return 0;
}


