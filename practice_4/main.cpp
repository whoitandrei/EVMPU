#include <iostream>
#include "libusb.h"
#include <stdio.h>
using namespace std;
void printdev(libusb_device *dev);

int main(){
	libusb_device **devs; // указатель на указатель на устройство,
	// используется для получения списка устройств
	libusb_context *ctx = NULL; // контекст сессии libusb
	int r; // для возвращаемых значений
	ssize_t cnt; // число найденных USB-устройств
	ssize_t i; // индексная переменная цикла перебора всех устройств
	// инициализировать библиотеку libusb, открыть сессию работы с libusb
	r = libusb_init(&ctx);
	if(r < 0){
		fprintf(stderr,
		"Ошибка: инициализация не выполнена, код: %d.\n", r);
		return 1;
	}
	// задать уровень подробности отладочных сообщений
	libusb_set_debug(ctx, 1);
	// получить список всех найденных USB- устройств
	cnt = libusb_get_device_list(ctx, &devs);
	if(cnt < 0){
		fprintf(stderr,
		"Error: USB device list not obtained.\n", r);
		return 1;
	}
	printf("devices found: %d\n", cnt);
	printf("=============================="
	"=============================\n");
	printf("* number of possible configurations\n");
	printf("|  * device class\n");
	printf("|  | * vendor ID\n");
	printf("|  |  | * device ID\n");
	printf("|  |  |    | * number of interfaces\n");
	printf("|  |  |    |    | * number of "
	"alternate settings\n");
	printf("|  |  |    |    |   | * device class\n");
	printf("|  |  |    |    |   |  | * interface number\n");
	printf("|  |  |    |    |   |  |  | * number of "
	"endpoints\n");
	printf("|  |  |    |    |   |  |  |  | * descriptor type\n");
	printf("|  |  |    |    |   |  |  |  |  | * endpoint "
	"address\n");
	printf("+--+--+----+----+---+--+--+--+"
	"--+--+----------------------\n");
	for(i = 0; i < cnt; i++) { // цикл перебора всех устройств
		printdev(devs[i]);
	}
	printf("=============================="
	"=============================\n");
	// освободить память, выделенную функцией получения списка устройств
	libusb_free_device_list(devs, 1);
	libusb_exit(ctx); // завершить работу с библиотекой libusb,
	// закрыть сессию работы с libusb
	return 0;
}
void printdev(libusb_device *dev){
	libusb_device_descriptor desc; // дескриптор устройства
	libusb_config_descriptor *config; // дескриптор конфигурации объекта
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0){
		fprintf(stderr,
		"error: descriptor of device didn't recieved. err code: %d.\n",r);
		return;
	}
	
	libusb_device_handle *handle;
	int opened = libusb_open(dev, &handle);
	
	if (desc.iSerialNumber && !opened) {
		unsigned char serial_number[256];
		r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serial_number, sizeof(serial_number));
		if (r < 0) {
			fprintf(stderr, "can't get serial number. code: %d\n", r);
		} else {
			printf("Serial Number: %s\n", serial_number);
		}
	} else {
		printf("serial number is not available.\n");
	}
	if (!opened)
		libusb_close(handle);
	

	
	// получить конфигурацию устройства
	libusb_get_config_descriptor(dev, 0, &config);
	printf(" %02x %02x %04x %04x %03x|  |  |  |  |  |\n",
	(int)desc.bNumConfigurations,
	(int)desc.bDeviceClass,
	desc.idVendor,
	desc.idProduct,
	(int)config->bNumInterfaces
	);
	for(int i=0; i<(int)config->bNumInterfaces; i++){
		inter = &config->interface[i];
		printf("|  |  |    |    |   "
		" %02x %02x|  |  |  |\n",
		inter->num_altsetting,
		(int)desc.bDeviceClass
		);
		for(int j=0; j<inter->num_altsetting; j++) {
			interdesc = &inter->altsetting[j];
			printf("|  |  |    |    |   |  |  "
			" %02x %02x|  |\n",
			(int)interdesc->bInterfaceNumber,
			(int)interdesc->bNumEndpoints
			);
			for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
				epdesc = &interdesc->endpoint[k];
				printf(
				"|  |  |    |    |   |  |  |  |  "
				" %02x %03x\n\n",
				(int)epdesc->bDescriptorType,
				(int)(int)epdesc->bEndpointAddress
				);
			}
		}
	}
	libusb_free_config_descriptor(config);
}