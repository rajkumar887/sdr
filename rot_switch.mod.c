#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x96558a33, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x659a4398, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x69f84ff7, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x9ceee68e, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xb3898c66, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x70240d85, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x6481563c, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xb8a534f6, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x4dee3544, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x75407533, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x8e865d3c, __VMLINUX_SYMBOL_STR(arm_delay_ops) },
	{ 0xc5f337e1, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0xfec55f01, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "431BDE53EF879C26E7A5CD0");
