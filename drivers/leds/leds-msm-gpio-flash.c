<<<<<<< HEAD
/* Copyright (c) 2013-2014, 2017-2018 The Linux Foundation. All rights reserved.
=======

/* Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
 * Copyright (C) 2018 XiaoMi, Inc.
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
<<<<<<< HEAD

=======
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/pinctrl/consumer.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/spinlock.h>

/* #define CONFIG_GPIO_FLASH_DEBUG */
=======
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/errno.h>
#include <linux/delay.h>

>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
#undef CDBG
#ifdef CONFIG_GPIO_FLASH_DEBUG
#define CDBG(fmt, args...) pr_err(fmt, ##args)
#else
#define CDBG(fmt, args...) do { } while (0)
#endif

#define LED_GPIO_FLASH_DRIVER_NAME	"qcom,leds-gpio-flash"
#define LED_TRIGGER_DEFAULT		"none"

#define GPIO_OUT_LOW          (0 << 1)
#define GPIO_OUT_HIGH         (1 << 1)
<<<<<<< HEAD

#define TORCH_ON       0x924B
#define TORCH_OFF      0x9259
#define TORCH_CURRENT_60       0x9649
#define TORCH_CURRENT_120      0x964B
#define TORCH_CURRENT_200      0x9659
#define TORCH_CURRENT_280      0x965B
#define FWD_WORD       0x92D9
#define CAMERA_FLASH   0x9249
#define FLASH_CURRENT_720      0xB2DB
#define FLASH_CURRENT_980      0xB64B
#define RESET                   0xB6DB

#define SEQ_NUM 2

static DEFINE_SPINLOCK(torch_lock);
=======
#define D6_GPIO50	      50
#define D3_PM8953_L22	      "pm8953_l22"
#define U_MIN_VOLTAGE 	       3300000
#define U_MAX_VOLTAGE	       3300000
#define U_OPT_VOLTAGE          2000000


static int l6210_flag = 0;

>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
enum msm_flash_seq_type_t {
	FLASH_EN,
	FLASH_NOW,
};

struct msm_flash_ctrl_seq {
	enum msm_flash_seq_type_t seq_type;
	uint8_t flash_on_val;
	uint8_t torch_on_val;
	uint8_t flash_off_val;
};

struct led_gpio_flash_data {
	int flash_en;
	int flash_now;
	int brightness;
	struct led_classdev cdev;
	struct pinctrl *pinctrl;
	struct pinctrl_state *gpio_state_default;
	struct msm_flash_ctrl_seq ctrl_seq[2];
};

static struct of_device_id led_gpio_flash_of_match[] = {
	{.compatible = LED_GPIO_FLASH_DRIVER_NAME,},
	{},
};

static void led_gpio_brightness_set(struct led_classdev *led_cdev,
				    enum led_brightness value)
{
<<<<<<< HEAD
	int rc = 0;
	int i = 0;
	int pattern;
	struct led_gpio_flash_data *flash_led =
	    container_of(led_cdev, struct led_gpio_flash_data, cdev);
	int brightness = value;

	spin_lock(&torch_lock);

	if (value != 0) {
		pattern = FWD_WORD;
		for (i = 0; i < 16; i++) {
			gpio_set_value(flash_led->flash_en,
				((pattern>>(15-i)) & 0x01));
			udelay(40);
		}
		gpio_set_value(flash_led->flash_en, 0);
		mdelay(1);
		if (value > LED_HALF)
			pattern = FLASH_CURRENT_980;
		else
			pattern = TORCH_CURRENT_200;
		for (i = 0; i < 16; i++) {
			gpio_set_value(flash_led->flash_en,
				((pattern>>(15-i)) & 0x01));
			udelay(40);
		}
		if (value > LED_HALF)
			pattern = CAMERA_FLASH;
		else
			pattern = TORCH_ON;
	} else {
		pattern = TORCH_OFF;
		for (i = 0; i < 16; i++) {
			gpio_set_value(flash_led->flash_en,
				((pattern>>(15-i)) & 0x01));
			udelay(40);
		}
		pattern = RESET;
	}
	gpio_set_value(flash_led->flash_en, 0);
	mdelay(1);
	for (i = 0; i < 16; i++) {
		gpio_set_value(flash_led->flash_en,
			((pattern>>(15-i)) & 0x01));
		udelay(40);
	}
	gpio_set_value(flash_led->flash_en, 0);
	mdelay(1);
	if (pattern == CAMERA_FLASH) {
		gpio_set_value(flash_led->flash_en, 1);
		udelay(40);
	} else {
		gpio_set_value(flash_led->flash_en, 0);
	}
	mdelay(1);
	spin_unlock(&torch_lock);
	if (rc) {
		pr_err("%s: Failed to set gpio %d\n", __func__,
=======
	int i,rc = 0;
	struct led_gpio_flash_data *flash_led =
	    container_of(led_cdev, struct led_gpio_flash_data, cdev);

	int brightness = value;
	int flash_en = 0;

	printk("lct flash_en=%d\n",flash_led->ctrl_seq[FLASH_EN].flash_on_val);
	if (brightness > 200) {
		flash_en =
			flash_led->ctrl_seq[FLASH_EN].flash_on_val;
	} else if (brightness > LED_OFF) {
		for(i = 0; i < 16; i++) {
			gpio_direction_output(35,0);
			mdelay(1);
			gpio_direction_output(35,1);
		}
		flash_en =
			flash_led->ctrl_seq[FLASH_EN].torch_on_val;
	} else {
		flash_en = 0;

	}
	printk("<3>""lct %s:brightness %d, flash_en=%d\n", __func__,
			brightness, flash_en);


	rc = gpio_direction_output(flash_led->flash_en, flash_en);
	if (rc) {
		printk("lct %s: Failed to set gpio %d\n", __func__,
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
		       flash_led->flash_en);
		goto err;
	}

<<<<<<< HEAD
	if (rc) {
		pr_err("%s: Failed to set gpio %d\n", __func__,
		       flash_led->flash_now);
		goto err;
=======
	if(l6210_flag) {
		rc = gpio_direction_output(flash_led->flash_now, flash_en);
		if (rc) {
			pr_err("lct %s: Failed to set gpio %d\n", __func__,
		        flash_led->flash_now);
		        goto err;
		}
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
	}
	flash_led->brightness = brightness;
err:
	return;
}

static enum led_brightness led_gpio_brightness_get(struct led_classdev
						   *led_cdev)
{
	struct led_gpio_flash_data *flash_led =
	    container_of(led_cdev, struct led_gpio_flash_data, cdev);
<<<<<<< HEAD

=======
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
	return flash_led->brightness;
}

static int led_gpio_flash_probe(struct platform_device *pdev)
{
	int rc = 0;
	const char *temp_str;
	struct led_gpio_flash_data *flash_led = NULL;
	struct device_node *node = pdev->dev.of_node;
	const char *seq_name = NULL;
<<<<<<< HEAD
	uint32_t array_flash_seq[SEQ_NUM];
	uint32_t array_torch_seq[SEQ_NUM];
	int i = 0;

	flash_led = devm_kzalloc(&pdev->dev, sizeof(struct led_gpio_flash_data),
				 GFP_KERNEL);
	if (flash_led == NULL)
		return -ENOMEM;
=======
	uint32_t array_flash_seq[2];
	uint32_t array_torch_seq[2];
	int i = 0;
	struct regulator *reg_ptr = NULL;


	flash_led = devm_kzalloc(&pdev->dev, sizeof(struct led_gpio_flash_data),
				 GFP_KERNEL);


	if (flash_led == NULL) {
		dev_err(&pdev->dev, "%s:%d Unable to allocate memory\n",
			__func__, __LINE__);
		return -ENOMEM;
	}
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite

	flash_led->cdev.default_trigger = LED_TRIGGER_DEFAULT;
	rc = of_property_read_string(node, "linux,default-trigger", &temp_str);
	if (!rc)
		flash_led->cdev.default_trigger = temp_str;

	flash_led->pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(flash_led->pinctrl)) {
<<<<<<< HEAD
		pr_err("%s:failed to get pinctrl\n", __func__);
=======
		printk("lct %s:failed to get pinctrl\n", __func__);
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
		return PTR_ERR(flash_led->pinctrl);
	}

	flash_led->gpio_state_default = pinctrl_lookup_state(flash_led->pinctrl,
		"flash_default");
	if (IS_ERR(flash_led->gpio_state_default)) {
<<<<<<< HEAD
		pr_err("%s:can not get active pinstate\n", __func__);
=======
		printk("lct %s:can not get active pinstate\n", __func__);
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
		return -EINVAL;
	}

	rc = pinctrl_select_state(flash_led->pinctrl,
		flash_led->gpio_state_default);
	if (rc)
<<<<<<< HEAD
		pr_err("%s:set state failed!\n", __func__);

	flash_led->flash_en = of_get_named_gpio(node, "qcom,flash-en", 0);
	if (flash_led->flash_en < 0) {
		dev_err(&pdev->dev,
			"Looking up %s property in node %s failed. rc =  %d\n",
			"qcom,flash-en", node->full_name, flash_led->flash_en);
=======
		printk("lct %s:set state failed!\n", __func__);

#if 1
	/**
	**open pm8953_ldo22
	**/
	/*
	name_id = kmalloc(sizeof(char*), GFP_KERNEL);
	if (name_id ==NULL){
		pr_err("%s:%d Unable to allocate memeory\n",
			__func__, __LINE__);
		goto kerror_id;
	}

	rc = of_property_read_string(node, "qcom,pm8953-enable", &name_id);
	if(rc < 0){
		cm895_flag  = -1;
		pr_err("%s:%d read qcom,pm8953-enable failed\n",
			__func__, __LINE__);
		//goto kerror_id;
	}
	else{
		cm895_flag = 1;
		printk("qcom,pm8953-enable=%s\n", name_id);
	}
	printk("hjl cm895_flag=%d\n", cm895_flag);

	if(cm895_flag){
	*/

		reg_ptr = kmalloc(sizeof(struct regulator*), GFP_KERNEL);
		if(reg_ptr ==NULL) {
			pr_err("%s:%d Unable to allocate memeory\n",
				__func__, __LINE__);
			goto kerror_ptr;
		}
		reg_ptr = regulator_get(NULL, D3_PM8953_L22);
		if (IS_ERR(reg_ptr)) {
			pr_err("lct %s: %s get failed\n", __func__, D3_PM8953_L22);
			reg_ptr = NULL;
			goto vreg_get_fail;
		}
		if (regulator_count_voltages(reg_ptr) > 0) {
			rc = regulator_set_voltage(reg_ptr, U_MIN_VOLTAGE, U_MAX_VOLTAGE);
			if (rc < 0) {
				pr_err("lct %s: %s set voltage failed\n",
					__func__, D3_PM8953_L22);
				goto vreg_set_voltage_fail;
		}
		rc = regulator_set_optimum_mode(reg_ptr, U_OPT_VOLTAGE);
		if (rc < 0) {
				pr_err("lct %s: %s set optimum mode failed\n",
					__func__, D3_PM8953_L22);
					goto vreg_set_opt_mode_fail;
			    }
		rc = regulator_enable(reg_ptr);
		if (rc < 0) {
			pr_err("lct %s: %s regulator_enable failed\n", __func__,
				D3_PM8953_L22);
			goto vreg_unconfig;
		}
	    }

#endif
	flash_led->flash_en = of_get_named_gpio(node, "qcom,flash-en", 0);
	if (flash_led->flash_en < 0) {
		dev_err(&pdev->dev,
			"lct Looking up %s property in node %s failed. rc =  %d\n",
			"flash-en", node->full_name, flash_led->flash_en);
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
		goto error;
	} else {
		rc = gpio_request(flash_led->flash_en, "FLASH_EN");
		if (rc) {
			dev_err(&pdev->dev,
<<<<<<< HEAD
				"%s: Failed to request gpio %d,rc = %d\n",
=======
				"lct %s: Failed to request gpio %d,rc = %d\n",
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
				__func__, flash_led->flash_en, rc);

			goto error;
		}
	}
<<<<<<< HEAD
	rc = gpio_direction_output(flash_led->flash_en, 0);

	flash_led->flash_now = of_get_named_gpio(node, "qcom,flash-now", 0);
	if (flash_led->flash_now < 0) {
		dev_err(&pdev->dev,
			"Looking up %s property in node %s failed. rc =  %d\n",
			"qcom,flash-now", node->full_name,
			flash_led->flash_now);
		goto error;
	} else {
		rc = gpio_request(flash_led->flash_now, "FLASH_NOW");
		if (rc) {
			dev_err(&pdev->dev,
				"%s: Failed to request gpio %d,rc = %d\n",
				__func__, flash_led->flash_now, rc);
			goto error;
=======

	flash_led->flash_now = of_get_named_gpio(node, "qcom,flash-now", 0);
	if (flash_led->flash_now < 0) {
		printk("lct %s: %d get qcom,flash-now failed\n",__func__, __LINE__);
		dev_err(&pdev->dev,
			"lct Looking up %s property in node %s failed. rc =  %d\n",
			"flash-now", node->full_name, flash_led->flash_now);
	} else {
		rc = gpio_request(flash_led->flash_now, "FLASH_NOW");
		if (rc) {
			l6210_flag = -1;
			dev_err(&pdev->dev,
				"lct %s: Failed to request gpio %d,rc = %d\n",
				__func__, flash_led->flash_now, rc);
			goto error;
		} else{
			l6210_flag = 1;
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
		}
	}

	rc = of_property_read_string(node, "linux,name", &flash_led->cdev.name);
	if (rc) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "%s: Failed to read linux,name. rc = %d\n",
=======
		dev_err(&pdev->dev, "lct %s: Failed to read linux name. rc = %d\n",
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
			__func__, rc);
		goto error;
	}

	rc = of_property_read_u32_array(node, "qcom,flash-seq-val",
		array_flash_seq, 2);

	if (rc < 0) {
<<<<<<< HEAD
		pr_err("%s get qcom,flash-seq-val failed %d\n",
=======
		printk("lct %s get flash op seq failed %d\n",
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
			__func__, __LINE__);
		goto error;
	}

	rc = of_property_read_u32_array(node, "qcom,torch-seq-val",
		array_torch_seq, 2);

	if (rc < 0) {
<<<<<<< HEAD
		pr_err("%s get qcom,torch-seq-val failed %d\n",
=======
		printk("lct %s get torch op seq failed %d\n",
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
			__func__, __LINE__);
		goto error;
	}

<<<<<<< HEAD
	for (i = 0; i < SEQ_NUM; i++) {
		rc = of_property_read_string_index(node,
			"qcom,op-seq", i,
			&seq_name);
		CDBG("%s seq_name[%d] = %s\n", __func__, i,
			seq_name);
		if (rc < 0)
			dev_err(&pdev->dev, "%s qcom,op-seq failed %d\n",
=======
	for (i = 0; i < 1; i++) {
		rc = of_property_read_string_index(node,
			"qcom,op-seq", i,
			&seq_name);
		if (rc < 0)
			dev_err(&pdev->dev, "%s failed %d\n",
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
				__func__, __LINE__);

		if (!strcmp(seq_name, "flash_en")) {
			flash_led->ctrl_seq[FLASH_EN].seq_type =
				FLASH_EN;
<<<<<<< HEAD
			CDBG("%s:%d seq_type[%d] %d\n", __func__, __LINE__,
				i, flash_led->ctrl_seq[FLASH_EN].seq_type);
			if (array_flash_seq[i] == 0)
				flash_led->ctrl_seq[FLASH_EN].flash_on_val =
					GPIO_OUT_LOW;
			else
				flash_led->ctrl_seq[FLASH_EN].flash_on_val =
					GPIO_OUT_HIGH;

			if (array_torch_seq[i] == 0)
				flash_led->ctrl_seq[FLASH_EN].torch_on_val =
					GPIO_OUT_LOW;
			else
				flash_led->ctrl_seq[FLASH_EN].torch_on_val =
					GPIO_OUT_HIGH;
		} else if (!strcmp(seq_name, "flash_now")) {
			flash_led->ctrl_seq[FLASH_NOW].seq_type =
				FLASH_NOW;
			CDBG("%s:%d seq_type[%d] %d\n", __func__, __LINE__,
				i, flash_led->ctrl_seq[i].seq_type);
			if (array_flash_seq[i] == 0)
				flash_led->ctrl_seq[FLASH_NOW].flash_on_val =
					GPIO_OUT_LOW;
			else
				flash_led->ctrl_seq[FLASH_NOW].flash_on_val =
					GPIO_OUT_HIGH;

			if (array_torch_seq[i] == 0)
				flash_led->ctrl_seq[FLASH_NOW].torch_on_val =
					GPIO_OUT_LOW;
			 else
				flash_led->ctrl_seq[FLASH_NOW].torch_on_val =
					GPIO_OUT_HIGH;
		}

=======
			printk("lct %s:%d seq_type[%d] %d\n", __func__, __LINE__,
				i, flash_led->ctrl_seq[FLASH_EN].seq_type);
			if (array_flash_seq[i] == 0) {
				flash_led->ctrl_seq[FLASH_EN].flash_on_val =
					GPIO_OUT_LOW;
			} else {
				flash_led->ctrl_seq[FLASH_EN].flash_on_val =
					GPIO_OUT_HIGH;
			}
			if (array_torch_seq[i] == 0) {
				flash_led->ctrl_seq[FLASH_EN].torch_on_val =
					GPIO_OUT_LOW;
			} else {
				flash_led->ctrl_seq[FLASH_EN].torch_on_val =
					GPIO_OUT_HIGH;
			}
		}


>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
	}

	platform_set_drvdata(pdev, flash_led);
	flash_led->cdev.max_brightness = LED_FULL;
	flash_led->cdev.brightness_set = led_gpio_brightness_set;
	flash_led->cdev.brightness_get = led_gpio_brightness_get;
<<<<<<< HEAD

	rc = led_classdev_register(&pdev->dev, &flash_led->cdev);
	if (rc) {
		dev_err(&pdev->dev, "%s: Failed to register led dev. rc = %d\n",
			__func__, rc);
		goto error;
	}
=======
	rc = led_classdev_register(&pdev->dev, &flash_led->cdev);
	if (rc) {
		dev_err(&pdev->dev, "lct %s: Failed to register led dev. rc = %d\n",
			__func__, rc);
		goto error;
	}
	printk("lct %s:probe successfully!\n", __func__);
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
	return 0;

error:
	if (IS_ERR(flash_led->pinctrl))
		devm_pinctrl_put(flash_led->pinctrl);
	devm_kfree(&pdev->dev, flash_led);
	return rc;
<<<<<<< HEAD
=======

kerror_ptr:
	kfree(reg_ptr);

vreg_unconfig:
	if (regulator_count_voltages(reg_ptr) > 0)
		regulator_set_optimum_mode(reg_ptr, 0);

vreg_set_opt_mode_fail:
	if (regulator_count_voltages(reg_ptr) > 0)
		regulator_set_voltage(reg_ptr, 0, U_MAX_VOLTAGE);

vreg_set_voltage_fail:
	regulator_put(reg_ptr);
	reg_ptr = NULL;

vreg_get_fail:
	return -ENODEV;
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
}

static int led_gpio_flash_remove(struct platform_device *pdev)
{
	struct led_gpio_flash_data *flash_led =
	    (struct led_gpio_flash_data *)platform_get_drvdata(pdev);
<<<<<<< HEAD

=======
>>>>>>> e0e87b3bd3a9a... drivers: leds: Import Xiaomi changes for ugglite
	if (IS_ERR(flash_led->pinctrl))
		devm_pinctrl_put(flash_led->pinctrl);
	led_classdev_unregister(&flash_led->cdev);
	devm_kfree(&pdev->dev, flash_led);
	return 0;
}

static struct platform_driver led_gpio_flash_driver = {
	.probe = led_gpio_flash_probe,
	.remove = led_gpio_flash_remove,
	.driver = {
		   .name = LED_GPIO_FLASH_DRIVER_NAME,
		   .owner = THIS_MODULE,
		   .of_match_table = led_gpio_flash_of_match,
		   }
};

static int __init led_gpio_flash_init(void)
{
	return platform_driver_register(&led_gpio_flash_driver);
}

static void __exit led_gpio_flash_exit(void)
{
	return platform_driver_unregister(&led_gpio_flash_driver);
}

late_initcall(led_gpio_flash_init);
module_exit(led_gpio_flash_exit);

MODULE_DESCRIPTION("QCOM GPIO LEDs driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("leds:leds-msm-gpio-flash");
