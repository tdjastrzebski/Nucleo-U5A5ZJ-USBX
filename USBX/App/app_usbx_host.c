/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_usbx_host.c
 * @author  MCD Application Team
 * @brief   USBX host applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_filex.h"
#include "my_printf.h"
#include "terminal_colors.h"
#include "trace.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define HAS_ALL_FLAGS(value, flags) (((value) & (flags)) == (flags))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static TX_THREAD ux_host_app_thread;

/* USER CODE BEGIN PV */
TX_EVENT_FLAGS_GROUP ux_app_EventFlags;
UX_HOST_CLASS_STORAGE* storage;
UX_HOST_CLASS_STORAGE_MEDIA* storage_media;
FX_MEDIA* media;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID app_ux_host_thread_entry(ULONG thread_input);
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);

extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

/* USER CODE BEGIN PFP */
static void _writeNewFile();
/* USER CODE END PFP */

/**
  * @brief  Application USBX Host Initialization.
  * @param  memory_ptr: memory pointer
  * @retval status
  */
UINT MX_USBX_Host_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;

  UCHAR *pointer;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_USBX_Host_Init0 */
  /* USER CODE END MX_USBX_Host_Init0 */

  /* USER CODE BEGIN MX_USBX_Host_Init 1 */

  /* USER CODE END MX_USBX_Host_Init 1 */

  /* Allocate the stack for host application main thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, UX_HOST_APP_THREAD_STACK_SIZE,
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_ALLOCATE_STACK_ERROR */
		return TX_POOL_ERROR;
    /* USER CODE END MAIN_THREAD_ALLOCATE_STACK_ERROR */
  }

  /* Create the host application main thread */
  if (tx_thread_create(&ux_host_app_thread, UX_HOST_APP_THREAD_NAME, app_ux_host_thread_entry,
                       0, pointer, UX_HOST_APP_THREAD_STACK_SIZE, UX_HOST_APP_THREAD_PRIO,
                       UX_HOST_APP_THREAD_PREEMPTION_THRESHOLD, UX_HOST_APP_THREAD_TIME_SLICE,
                       UX_HOST_APP_THREAD_START_OPTION) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_CREATE_ERROR */
		return TX_THREAD_ERROR;
    /* USER CODE END MAIN_THREAD_CREATE_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Host_Init 2 */
	if (tx_event_flags_create(&ux_app_EventFlags, "Event Flag") != TX_SUCCESS) {
		return TX_GROUP_ERROR;
	}
  /* USER CODE END MX_USBX_Host_Init 2 */

  return ret;
}

/**
  * @brief  Function implementing app_ux_host_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
static VOID app_ux_host_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN app_ux_host_thread_entry */
	TX_PARAMETER_NOT_USED(thread_input);
	MX_USB_OTG_HS_HCD_Init();

	while (true) {
		ULONG txFlags;
		UINT txStatus = tx_event_flags_get(&ux_app_EventFlags, 0xff, TX_OR, &txFlags, TX_WAIT_FOREVER);  // wait for any flag, do not clear yet
		UNUSED(txStatus);

		if (HAS_ALL_FLAGS(txFlags, 0x03)) {
			// USBSTACK STOP/START - ignore
		} else if (HAS_ALL_FLAGS(txFlags, 0x01)) {
			// USBSTACK_START
			UINT result1 = MX_USBX_Host_Stack_Init();

			if (result1 == UX_SUCCESS) {
				HAL_StatusTypeDef result2 = HAL_HCD_Start(&hhcd_USB_OTG_HS);

				if (result2 != HAL_OK) {
					my_printf(RED("HAL_HCD_Start() error\n"));
				} else {
					/// HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_SET);
					my_printf(GREEN("USBX Host started\n"));
				}
			} else {
				my_printf(RED("MX_USBX_Host_Stack_Init() error\n"));
			}

			trace_HCD(1);
		} else if (HAS_ALL_FLAGS(txFlags, 0x02)) {
			// USBSTACK_STOP
			// HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_RESET);
			HAL_StatusTypeDef result1 = HAL_HCD_Stop(&hhcd_USB_OTG_HS);

			if (result1 != HAL_OK) {
				my_printf(RED("HAL_HCD_Stop() error\n"));
			} else {
				UINT result2 = MX_USBX_Host_Stack_DeInit();
				if (result2 != UX_SUCCESS) {
					my_printf(RED("MX_USBX_Host_Stack_DeInit() error\n"));
				} else {
					my_printf(GREEN("USBX Host stopped\n"));
				}
			}

			trace_HCD(1);
		}

		txStatus = tx_event_flags_get(&ux_app_EventFlags, 0xff, TX_OR_CLEAR, &txFlags, TX_NO_WAIT);  // clear flags, accept next notification
		tx_thread_sleep(MS_TO_TICK(10));
	}

  /* USER CODE END app_ux_host_thread_entry */
}

/**
  * @brief  ux_host_event_callback
  *         This callback is invoked to notify application of instance changes.
  * @param  event: event code.
  * @param  current_class: Pointer to class.
  * @param  current_instance: Pointer to class instance.
  * @retval status
  */
UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN ux_host_event_callback0 */
	UX_PARAMETER_NOT_USED(current_class);
	UX_PARAMETER_NOT_USED(current_instance);
  /* USER CODE END ux_host_event_callback0 */

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* USER CODE BEGIN UX_DEVICE_INSERTION */
		my_printf(CYAN("* UX_DEVICE_INSERTION *") "\n");
		if (current_class->ux_host_class_entry_function == ux_host_class_storage_entry) {
			if (storage == UX_NULL) {
				/* Get current Storage Instance */
				storage = (UX_HOST_CLASS_STORAGE*)current_instance;

				my_printf("\nUSB Mass Storage Device Plugged");
				my_printf("PID: %#x ", (UINT)storage->ux_host_class_storage_device->ux_device_descriptor.idProduct);
				my_printf("VID: %#x ", (UINT)storage->ux_host_class_storage_device->ux_device_descriptor.idVendor);

				/* Get the storage media */
				storage_media = (UX_HOST_CLASS_STORAGE_MEDIA*)current_class->ux_host_class_media;

				if (storage_media->ux_host_class_storage_media_lun != 0) {
					storage_media = UX_NULL;
				} else {
					/* Get the media file */
					media = &storage_media->ux_host_class_storage_media;
				}

				/* Check the storage class state */
				if (storage->ux_host_class_storage_state == UX_HOST_CLASS_INSTANCE_LIVE) {
					/* Set STORAGE_MEDIA flag */
					_writeNewFile();
				}
			}
		}
      /* USER CODE END UX_DEVICE_INSERTION */

      break;

    case UX_DEVICE_REMOVAL:

      /* USER CODE BEGIN UX_DEVICE_REMOVAL */
		my_printf(CYAN("* UX_DEVICE_REMOVAL *") "\n");
		if ((VOID*)storage == current_instance) {
			/* Clear storage media instance & media file */
			storage = UX_NULL;
			storage_media = UX_NULL;
			media = UX_NULL;

			my_printf("\nUSB Mass Storage Device Unplugged");
		}
      /* USER CODE END UX_DEVICE_REMOVAL */

      break;

    case UX_DEVICE_CONNECTION:

      /* USER CODE BEGIN UX_DEVICE_CONNECTION */
		my_printf(CYAN("* UX_DEVICE_CONNECTION *") "\n");
      /* USER CODE END UX_DEVICE_CONNECTION */

      break;

    case UX_DEVICE_DISCONNECTION:

      /* USER CODE BEGIN UX_DEVICE_DISCONNECTION */
		my_printf(CYAN("* UX_DEVICE_DISCONNECTION *") "\n");
      /* USER CODE END UX_DEVICE_DISCONNECTION */

      break;

    default:

      /* USER CODE BEGIN EVENT_DEFAULT */
		my_printf(CYAN("* UX_DEVICE unhandled event code: %li *") "\n", event);
      /* USER CODE END EVENT_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_event_callback1 */

  /* USER CODE END ux_host_event_callback1 */

  return status;
}

/**
  * @brief  ux_host_error_callback
  *         This callback is invoked to notify application of error changes.
  * @param  system_level: system level parameter.
  * @param  system_context: system context code.
  * @param  error_code: error event code.
  * @retval None
  */
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  /* USER CODE BEGIN ux_host_error_callback0 */
	UX_PARAMETER_NOT_USED(system_level);
	UX_PARAMETER_NOT_USED(system_context);
  /* USER CODE END ux_host_error_callback0 */

  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE:

      /* USER CODE BEGIN UX_DEVICE_ENUMERATION_FAILURE */
	  my_printf(BLUE("* UX_DEVICE_ENUMERATION_FAILURE *") "\n");
      /* USER CODE END UX_DEVICE_ENUMERATION_FAILURE */

      break;

    case  UX_NO_DEVICE_CONNECTED:

      /* USER CODE BEGIN UX_NO_DEVICE_CONNECTED */
	  my_printf(BLUE("* UX_NO_DEVICE_CONNECTED *") "\n");
      /* USER CODE END UX_NO_DEVICE_CONNECTED */

      break;

    default:

      /* USER CODE BEGIN ERROR_DEFAULT */
	  my_printf(BLUE("* Unhandled ux_host_error code: %li *\n"), error_code);
      /* USER CODE END ERROR_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_error_callback1 */

  /* USER CODE END ux_host_error_callback1 */
}

/**
  * @brief  MX_USBX_Host_Stack_Init
  *         Initialization of USB host stack.
  *         Init USB Host stack, add register the host class stack
  * @retval ret
  */
UINT MX_USBX_Host_Stack_Init(void)
{
  UINT ret = UX_SUCCESS;
  /* USER CODE BEGIN MX_USBX_Host_Stack_Init_PreTreatment_0 */
  /* USER CODE END MX_USBX_Host_Stack_Init_PreTreatment_0 */

  /* The code below is required for installing the host portion of USBX.  */
  if (ux_host_stack_initialize(ux_host_event_callback) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);

  /* Initialize the host storage class */
  if (ux_host_stack_class_register(_ux_system_host_class_storage_name,
                                   ux_host_class_storage_entry) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_STORAGE_REGISTER_ERROR */
		return UX_ERROR;
    /* USER CODE END USBX_HOST_STORAGE_REGISTER_ERROR */
  }

  /* Register all the USB host controllers available in this system. */
  if (ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                             _ux_hcd_stm32_initialize, USB_OTG_HS_BASE,
                             (ULONG)&hhcd_USB_OTG_HS)!= UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_STACK_HCD_REGISTER_ERROR */
		return UX_ERROR;
    /* USER CODE END USBX_HOST_STACK_HCD_REGISTER_ERROR */
  }
  /* USER CODE BEGIN MX_USBX_Host_Stack_Init_PreTreatment_1 */
  /* USER CODE END MX_USBX_Host_Stack_Init_PreTreatment_1 */

  /* USER CODE BEGIN MX_USBX_Host_Stack_Init_PostTreatment */
  /* USER CODE END MX_USBX_Host_Stack_Init_PostTreatment */
  return ret ;
}

/**
  * @brief  MX_USBX_Host_Stack_DeInit
  *         Uninitialize of USB Host stack.
  *         Uninitialize the host stack, unregister of host class stack and
  *         unregister of the usb host controllers
  * @retval ret
  */
UINT MX_USBX_Host_Stack_DeInit(void)
{
  UINT ret = UX_SUCCESS;

  /* USER CODE BEGIN MX_USBX_Host_Stack_DeInit_PreTreatment_0 */

  /* USER CODE END MX_USBX_Host_Stack_DeInit_PreTreatment_0 */

  /* Unregister all the USB host controllers available in this system. */
    if (ux_host_stack_hcd_unregister(_ux_system_host_hcd_stm32_name,
                               USB_OTG_HS_BASE,
                               (ULONG)&hhcd_USB_OTG_HS)!= UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Unregister the host storage class */
  if (ux_host_stack_class_unregister(ux_host_class_storage_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* The code below is required for uninstalling the host portion of USBX.  */
  if (ux_host_stack_uninitialize() != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* USER CODE BEGIN MX_USBX_Host_Stack_DeInit_PreTreatment_1 */
  /* USER CODE END MX_USBX_Host_Stack_DeInit_PreTreatment_1 */

  /* USER CODE BEGIN MX_USBX_Host_Stack_DeInit_PostTreatment */
  /* USER CODE END MX_USBX_Host_Stack_DeInit_PostTreatment */
  return ret ;
}
/* USER CODE BEGIN 1 */
static void _writeNewFile() {
	/* Start file operations once the media is connected */
	if (media != NULL) {
		/* Start File operations */
		my_printf("\n*** Start Files operations ***\n");

		/* Create a file and check status */
		if (App_File_Create(media) == UX_SUCCESS) {
			my_printf("File TEST.TXT Created \n");

			/* Start write File Operation */
			my_printf("Write Process ...... \n");

			/* Check status */
			if (App_File_Write(media) == UX_SUCCESS) {
				my_printf("Write Process Success \n");

				/* Start Read File Operation and comparison operation */
				my_printf("Read Process  ...... \n");

				/* Check Read Operation */
				if (App_File_Read(media) == UX_SUCCESS) {
					my_printf("Read Process Success  \n");
					my_printf("File Closed \n");
					my_printf("*** End Files operations ***\n");
				} else {
					my_printf("!! Read Process Fail !! \n");
				}
			} else {
				my_printf("!! Write Process Fail !! ");
			}

		} else {
			my_printf(" !! Could Not Create TEST.TXT File !! ");
		}
	} else {
		tx_thread_sleep(MS_TO_TICK(10));
	}
}

/* USER CODE END 1 */
