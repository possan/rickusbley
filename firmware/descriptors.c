#include "descriptors.h"

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(1,1,0),
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x03EB,
	.ProductID              = 0x2047,
	.ReleaseNumber          = VERSION_BCD(0,0,2),

	.ManufacturerStrIndex   = STRING_ID_Manufacturer,
	.ProductStrIndex        = STRING_ID_Product,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Config =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize   = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces          = 2,

			.ConfigurationNumber      = 1,
			.ConfigurationStrIndex    = NO_DESCRIPTOR,

			.ConfigAttributes         = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

			.MaxPowerConsumption      = USB_CONFIG_POWER_MA(100)
		},

	.Audio_ControlInterface =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber          = INTERFACE_ID_AudioControl,
			.AlternateSetting         = 0,

			.TotalEndpoints           = 0,

			.Class                    = AUDIO_CSCP_AudioClass,
			.SubClass                 = AUDIO_CSCP_ControlSubclass,
			.Protocol                 = AUDIO_CSCP_ControlProtocol,

			.InterfaceStrIndex        = NO_DESCRIPTOR
		},

	.Audio_ControlInterface_SPC =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_Interface_AC_t), .Type = AUDIO_DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_Header,

			.ACSpecification          = VERSION_BCD(1,0,0),
			.TotalLength              = (sizeof(USB_Audio_Descriptor_Interface_AC_t) +
			                             sizeof(USB_Audio_Descriptor_InputTerminal_t) +
			                             sizeof(USB_Audio_Descriptor_OutputTerminal_t)),

			.InCollection             = 1,
			.InterfaceNumber          = INTERFACE_ID_AudioStream,
		},

	.Audio_InputTerminal =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_InputTerminal_t), .Type = AUDIO_DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_InputTerminal,

			.TerminalID               = 0x01,
			.TerminalType             = AUDIO_TERMINAL_IN_MIC,
			.AssociatedOutputTerminal = 0x00,

			.TotalChannels            = 1,
			.ChannelConfig            = 0,

			.ChannelStrIndex          = NO_DESCRIPTOR,
			.TerminalStrIndex         = NO_DESCRIPTOR
		},

	.Audio_OutputTerminal =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_OutputTerminal_t), .Type = AUDIO_DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_OutputTerminal,

			.TerminalID               = 0x02,
			.TerminalType             = AUDIO_TERMINAL_STREAMING,
			.AssociatedInputTerminal  = 0x00,

			.SourceID                 = 0x01,

			.TerminalStrIndex         = NO_DESCRIPTOR
		},

	.Audio_StreamInterface_Alt0 =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber          = INTERFACE_ID_AudioStream,
			.AlternateSetting         = 0,

			.TotalEndpoints           = 0,

			.Class                    = AUDIO_CSCP_AudioClass,
			.SubClass                 = AUDIO_CSCP_AudioStreamingSubclass,
			.Protocol                 = AUDIO_CSCP_StreamingProtocol,

			.InterfaceStrIndex        = NO_DESCRIPTOR
		},

	.Audio_StreamInterface_Alt1 =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber          = INTERFACE_ID_AudioStream,
			.AlternateSetting         = 1,

			.TotalEndpoints           = 1,

			.Class                    = AUDIO_CSCP_AudioClass,
			.SubClass                 = AUDIO_CSCP_AudioStreamingSubclass,
			.Protocol                 = AUDIO_CSCP_StreamingProtocol,

			.InterfaceStrIndex        = NO_DESCRIPTOR
		},

	.Audio_StreamInterface_SPC =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_Interface_AS_t), .Type = AUDIO_DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_General,

			.TerminalLink             = 0x02,

			.FrameDelay               = 1,
			.AudioFormat              = 0x0001
		},

	.Audio_AudioFormat =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_Format_t) +
			                                     sizeof(ConfigurationDescriptor.Audio_AudioFormatSampleRates),
			                             .Type = AUDIO_DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_FormatType,

			.FormatType               = 0x01,
			.Channels                 = 0x01,

			.SubFrameSize             = 0x02,
			.BitResolution            = 8, // 16,

			.TotalDiscreteSampleRates = 4,
		},

	.Audio_AudioFormatSampleRates =
		{
			AUDIO_SAMPLE_FREQ(8000),
			AUDIO_SAMPLE_FREQ(11025),
			AUDIO_SAMPLE_FREQ(22050),
			AUDIO_SAMPLE_FREQ(44100),
			// AUDIO_SAMPLE_FREQ(48000),
		},

	.Audio_StreamEndpoint =
		{
			.Endpoint =
				{
					.Header              = {.Size = sizeof(USB_Audio_Descriptor_StreamEndpoint_Std_t), .Type = DTYPE_Endpoint},

					.EndpointAddress     = AUDIO_STREAM_EPADDR,
					.Attributes          = (EP_TYPE_ISOCHRONOUS | ENDPOINT_ATTR_SYNC | ENDPOINT_USAGE_DATA),
					.EndpointSize        = AUDIO_STREAM_EPSIZE,
					.PollingIntervalMS   = 0x01
				},

			.Refresh                  = 0,
			.SyncEndpointNumber       = 0
		},

	.Audio_StreamEndpoint_SPC =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_StreamEndpoint_Spc_t), .Type = AUDIO_DTYPE_CSEndpoint},
			.Subtype                  = AUDIO_DSUBTYPE_CSEndpoint_General,

			.Attributes               = (AUDIO_EP_ACCEPTS_SMALL_PACKETS | AUDIO_EP_SAMPLE_FREQ_CONTROL),

			.LockDelayUnits           = 0x00,
			.LockDelay                = 0x0000
		}
};

const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"Possan");
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"Rick USBley");

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device:
			Address = &DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String:
			switch (DescriptorNumber)
			{
				case STRING_ID_Language:
					Address = &LanguageString;
					Size    = pgm_read_byte(&LanguageString.Header.Size);
					break;
				case STRING_ID_Manufacturer:
					Address = &ManufacturerString;
					Size    = pgm_read_byte(&ManufacturerString.Header.Size);
					break;
				case STRING_ID_Product:
					Address = &ProductString;
					Size    = pgm_read_byte(&ProductString.Header.Size);
					break;
			}

			break;
	}

	*DescriptorAddress = Address;
	return Size;
}

