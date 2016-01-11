/* 
 * drivers/misc/pocket_mode.c
 *
 * Copyright (C) 2015 Avinaba Dalal <d97.avinaba@gmail.com>
 *
 * Generic unit to handle most of the pocket_mode related functions.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, and only version 2, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/kernel.h>

#define UNIT_MAJOR_VERSION 		1
#define UNIT_MINOR_VERSION 		0

#define MASTER_SWITCH			1		//enable pocket_mode by default

pocket_mode_switch = MASTER_SWITCH;

static ssize_t pocket_mode_switch_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d", pocket_mode_switch);
}

static ssize_t pocket_mode_switch_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int new_switch;
	sscanf(buf, "%d", &new_switch);
	if(new_switch != 0 && new_switch != 1)
		pocket_mode_switch = MASTER_SWITCH;
	else
		pocket_mode_switch = new_switch;
	
	return count;
}

static ssize_t pocket_mode_version_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "Pocket Mode %u.%u", UNIT_MAJOR_VERSION, UNIT_MINOR_VERSION);
} 

static struct kobj_attribute pocket_mode_ver_attribute =
	__ATTR(version,
		0444,
		pocket_mode_version_show, NULL);

static struct kobj_attribute pocket_mode_switch_attribute =
	__ATTR(pocket_mode,
		0666,
		pocket_mode_switch_show,
		pocket_mode_switch_store);

static struct attribute *pocket_mode_attrs[] =
	{
		&pocket_mode_switch_attribute.attr,
		&pocket_mode_ver_attribute.attr,
		NULL,
	};

static struct attribute_group pocket_mode_attr_group =
	{
		.attrs = pocket_mode_attrs,
	};

static struct kobject *pocket_mode_kobj;

static int pocket_mode_probe(void)
{
	int sysfs_result;
	printk(KERN_DEBUG "[%s]\n",__func__);

	pocket_mode_kobj = kobject_create_and_add("dt2w", kernel_kobj);

	if (!pocket_mode_kobj) {
		pr_err("%s Interface create failed!\n",
			__FUNCTION__);
		return -ENOMEM;
        }

	sysfs_result = sysfs_create_group(pocket_mode_kobj,
			&pocket_mode_attr_group);

	if (sysfs_result) {
		pr_info("%s sysfs create failed!\n", __FUNCTION__);
		kobject_put(pocket_mode_kobj);
	}
	return sysfs_result;
}

static void pocket_mode_remove(void)
{
	if (pocket_mode_kobj != NULL)
		kobject_put(pocket_mode_kobj);
}

module_init(pocket_mode_probe);
module_exit(pocket_mode_remove);
MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Avinaba Dalal <d97.avinaba@gmail.com>");
MODULE_DESCRIPTION("Generic unit to handle most of the pocket_mode related functions.");

