#include <units.h>
#include <delay.h>
#include <Stepper.h>



void main(){
	while(1){
		Move_To(-360);
		Move_To(360);
	}
}
