#include <kernel/iobus.h>
#include <stdio.h>
#include <stdint.h>

#define PCI_CONFIG_ADDRESS	0xCF8
#define PCI_CONFIG_DATA		0xCFC

static uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
             (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write out the address */
    outportl (PCI_CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((inportl (PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

static uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
    //printf("Trying bus %i slot %i\n",bus,slot);
    uint16_t vendor, device, class;
    /* try and read the first configuration register. Since there are no */
    /* vendors that == 0xFFFF, it must be a non-existent device. */
    //if ((vendor = pciConfigReadWord(bus,slot,0,0)) != 0xFFFF) {
    if ((vendor = pciConfigReadWord(bus,slot,0,0)) == 0x8086) {
        /* see http://pcidatabase.com/vendor_details.php?id=1302
	 * 8086 = Intel
	 * devices: 1237,7000,100e
	 * PCI & Memory
	 * PIIX3 PCI-to-ISA Bridge: this one is our disk driver under qemu
	 * Intel Pro 1000/MT
	 */
        device = pciConfigReadWord(bus,slot,0,2);
        class = pciConfigReadWord(bus,slot,0,8);
	printf("Vendor:0x%x\n",vendor);
	printf(" Device:0x%x\n",device);
        printf(" Class/Subclass:0x%x\n", class);
        printf(" Class/Subclass:0x%x 0x%x\n", (uint8_t) (class >> 8), (uint8_t)(class));
    } return (vendor);
}

void checkAllBuses(void) {
    uint8_t bus;
    uint8_t device;

    for(bus = 0; bus < 255; bus++) {
        for(device = 0; device < 32; device++) {
            pciCheckVendor(bus, device);
        }
    }
}
