#include "audioinput.h"
#include "roll.h"

static bool StreamingAudioInterfaceSelected = false;

volatile static uint32_t CurrentAudioSampleFrequency = 44100;

// #define SHAPE_SIZE 256
// volatile static int16_t shape[SHAPE_SIZE];
// #define RINGBUFFER_SIZE 7
// volatile static int16_t rb_buffer[RINGBUFFER_SIZE];
// volatile static int16_t rb_readpos = 0;
// volatile static int16_t rb_writepos = 0;
// volatile static int16_t rb_fillrate = 0;

// volatile static uint32_t Phase = 0;

static void fillRingbuffer() {
	// if (rb_fillrate < 200) {
	// return;
	// }

	// if (rb_fillrate < 0) {
	// 	rb_fillrate = 0;
	// }

	// // float hz = 10.0;
	// // uint32_t rate = 900;// (uint32_t)((hz * 512.0f * 1024.0f) / (float)CurrentAudioSampleFrequency);

	// int16_t tempo[50];
	// for(int j=0; j<2; j++) {

	// 	librick_render(&tempo, 50);
	// 	for(int k=0; k<50; k++) {

	// 		// Phase += rate;
	// 		// if (Phase > (512 * 1024L)) {
	// 		// 	Phase -= (512 * 1024L);
	// 		// }
	// 		// Phase &= ((512 * 1024) - 1);
	// 		// while (Phase > (512 * 1024)) { Phase -= (512 * 1024); }
	// 		// Phase &= 0x7fff;
	// 		// int16_t AudioSample = 0;
	// 		// librick_render(&AudioSample, 1);
	// 		// // AudioSample = shape[(Phase >> 10) & 511];//  > 0x4000 ? 30000 : -30000; //  Phase & 0x7fff;

	// 		rb_buffer[rb_writepos] = tempo[k];
	// 		rb_writepos ++;

	// 		if (rb_writepos >= RINGBUFFER_SIZE) {
	// 			rb_writepos = 0;
	// 		}

	// 		rb_fillrate ++;
	// 	}
	// }
}

int main(void)
{
	SetupHardware();

	// for(int i=0; i<SHAPE_SIZE; i++) {
	// 	// shape[i] = -32000 + (64000 * i / 512);
	// 	shape[i] = 32000 * sin((float)i * 3.14259f / ((float)SHAPE_SIZE / 2.0f)); // (64000 * i / 512);
	// }

	// for(int i=0; i<RINGBUFFER_SIZE; i++) {
	// 	// rb_buffer[i] = (25000 * i) / RINGBUFFER_SIZE; //
	// 	rb_buffer[i] = rand() % 30000;
	// }

	// rb_fillrate = 0;
	// rb_readpos = 256;
	// rb_writepos = 0;

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();

	for (;;)
	{
		USB_USBTask();
		fillRingbuffer();
	}
}

void SetupHardware(void)
{
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	clock_prescale_set(clock_div_1);
	LEDs_Init();
	USB_Init();
}

void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);

	librick_init(CurrentAudioSampleFrequency);

	/* Sample reload timer initialization */
	TIMSK0  = (1 << OCIE0A);
	OCR0A   = ((F_CPU / 8 / (CurrentAudioSampleFrequency / 1)) - 1);
	TCCR0A  = (1 << WGM01);  // CTC mode
	TCCR0B  = (1 << CS01);   // Fcpu/8 speed
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs, disables the sample update and PWM output timers and stops the USB and Audio management tasks.
 */
void EVENT_USB_Device_Disconnect(void)
{
	/* Stop the sample reload timer */
	TCCR0B = 0;

	/* Indicate streaming audio interface not selected */
	StreamingAudioInterfaceSelected = false;

	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup Audio Stream Endpoint */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(AUDIO_STREAM_EPADDR, EP_TYPE_ISOCHRONOUS, AUDIO_STREAM_EPSIZE, 2);

	/* Indicate endpoint configuration success or failure */
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
	/* Process General and Audio specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		case REQ_SetInterface:
			/* Set Interface is not handled by the library, as its function is application-specific */
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();

				/* Check if the host is enabling the audio interface (setting AlternateSetting to 1) */
				StreamingAudioInterfaceSelected = ((USB_ControlRequest.wValue) != 0);
			}

			break;
		case AUDIO_REQ_GetStatus:
			/* Get Status request can be directed at either the interface or endpoint, neither is currently used
			 * according to the latest USB Audio 1.0 standard, but must be ACKed with no data when requested */
			if ((USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) ||
			    (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_ENDPOINT)))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
			}

			break;
		case AUDIO_REQ_SetCurrent:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_ENDPOINT))
			{
				/* Extract out the relevant request information to get the target Endpoint address and control being set */
				uint8_t EndpointAddress = (uint8_t)USB_ControlRequest.wIndex;
				uint8_t EndpointControl = (USB_ControlRequest.wValue >> 8);

				/* Only handle SET CURRENT requests to the audio endpoint's sample frequency property */
				if ((EndpointAddress == AUDIO_STREAM_EPADDR) && (EndpointControl == AUDIO_EPCONTROL_SamplingFreq))
				{
					uint8_t SampleRate[3];

					Endpoint_ClearSETUP();
					Endpoint_Read_Control_Stream_LE(SampleRate, sizeof(SampleRate));
					Endpoint_ClearIN();

					/* Set the new sampling frequency to the value given by the host */
					CurrentAudioSampleFrequency = (((uint32_t)SampleRate[2] << 16) | ((uint32_t)SampleRate[1] << 8) | (uint32_t)SampleRate[0]);

					librick_init(CurrentAudioSampleFrequency);

					/* Adjust sample reload timer to the new frequency */
					OCR0A = ((F_CPU / 8 / (CurrentAudioSampleFrequency / 1)) - 1);
				}
			}

			break;
		case AUDIO_REQ_GetCurrent:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_ENDPOINT))
			{
				/* Extract out the relevant request information to get the target Endpoint address and control being retrieved */
				uint8_t EndpointAddress = (uint8_t)USB_ControlRequest.wIndex;
				uint8_t EndpointControl = (USB_ControlRequest.wValue >> 8);

				/* Only handle GET CURRENT requests to the audio endpoint's sample frequency property */
				if ((EndpointAddress == AUDIO_STREAM_EPADDR) && (EndpointControl == AUDIO_EPCONTROL_SamplingFreq))
				{
					uint8_t SampleRate[3];

					/* Convert the sampling rate value into the 24-bit format the host expects for the property */
					SampleRate[2] = (CurrentAudioSampleFrequency >> 16);
					SampleRate[1] = (CurrentAudioSampleFrequency >> 8);
					SampleRate[0] = (CurrentAudioSampleFrequency &  0xFF);

					Endpoint_ClearSETUP();
					Endpoint_Write_Control_Stream_LE(SampleRate, sizeof(SampleRate));
					Endpoint_ClearOUT();
				}
			}

			break;
	}
}

// static long counter;

// static int16_t nextsample = 0;

ISR(TIMER0_COMPA_vect, ISR_BLOCK)
{
	uint8_t PrevEndpoint = Endpoint_GetCurrentEndpoint();
	Endpoint_SelectEndpoint(AUDIO_STREAM_EPADDR);

	if (!(Endpoint_IsReadWriteAllowed()))
	{
	// 	/* Clear the endpoint bank to send its contents to the host */
		Endpoint_ClearIN();

		/* Wait until the endpoint is ready for more data */
		// if (Endpoint_WaitUntilReady())
		// 	return;
	}

	if (Endpoint_IsINReady() && StreamingAudioInterfaceSelected) {
		int8_t sample[64] = { 0, };
		librick_render(&sample, 32);
		for(int k=0; k<32; k++) {
			Endpoint_Write_8(sample[k]);
		}
		// if (!(Endpoint_IsReadWriteAllowed())) {
		// Endpoint_ClearIN();
			// break;
		// }
	}
	// }

	Endpoint_SelectEndpoint(PrevEndpoint);
	// librick_render(&nextsample, 1);
}

