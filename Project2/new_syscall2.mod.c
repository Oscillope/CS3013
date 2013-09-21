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
	{ 0x6713b004, "module_layout" },
	{ 0x268cc6a2, "sys_close" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x7f24de73, "jiffies_to_usecs" },
	{ 0x50eedeb8, "printk" },
	{ 0xdbad3c1, "current_task" },
	{ 0x23a39dcd, "pv_cpu_ops" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8287ADE492DA6D3735EDE2E");
