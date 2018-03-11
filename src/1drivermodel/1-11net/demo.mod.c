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
	{ 0xbca7617b, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x3e37d1cb, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x37196217, __VMLINUX_SYMBOL_STR(alloc_netdev_mqs) },
	{ 0x45acd268, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0x36de9ce, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x230e657d, __VMLINUX_SYMBOL_STR(consume_skb) },
	{ 0x7cff325e, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0x734b1ca6, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xf2b63e2a, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0xe4782dc0, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0x49ebacbd, __VMLINUX_SYMBOL_STR(_clear_bit) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x28a19541, __VMLINUX_SYMBOL_STR(ether_setup) },
	{ 0x51d559d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x598542b2, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x1926eaa3, __VMLINUX_SYMBOL_STR(netif_tx_wake_queue) },
	{ 0x676bbc0f, __VMLINUX_SYMBOL_STR(_set_bit) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A51D4193AD6859CC358467D");
