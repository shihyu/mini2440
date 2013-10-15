#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0xb9d3067e, "module_layout" },
	{ 0x3e18b1c5, "__mutex_init" },
	{ 0x6a1ceaf2, "dev_set_drvdata" },
	{ 0xd8c3be51, "usb_get_dev" },
	{ 0x22766ce1, "usb_register_dev" },
	{ 0x8a1ae636, "mutex_trylock" },
	{ 0xa2e429a9, "kmem_cache_alloc_trace" },
	{ 0xd83ea029, "kmalloc_caches" },
	{ 0x9f3dd05, "usb_find_interface" },
	{ 0xdeb87602, "usb_register_driver" },
	{ 0xec6c52c, "usb_deregister_dev" },
	{ 0x466649c3, "dev_get_drvdata" },
	{ 0xcbdb9f8, "usb_bulk_msg" },
	{ 0x4f6b400b, "_copy_from_user" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x9297e8ae, "mutex_unlock" },
	{ 0x37a0cba, "kfree" },
	{ 0x27e1a049, "printk" },
	{ 0x562b9321, "usb_deregister" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EADAAA8B0D15E679A64541A");
