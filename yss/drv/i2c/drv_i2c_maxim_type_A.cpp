////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_2.0
// 본 소스코드의 소유권은 yss Embedded Operating System 네이버 카페 관리자와 운영진에게 있습니다.
// 운영진이 임의로 코드의 권한을 타인에게 양도할 수 없습니다.
// 본 소스코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스코드의 상업적 또는 비상업적 이용이 가능합니다.
// 본 소스코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스코드의 내용을 무단 전재하는 행위를 금합니다.
// 본 소스코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떤한 법적 책임을 지지 않습니다.
//
//	Home Page : http://cafe.naver.com/yssoperatingsystem
//	Copyright 2020.	yss Embedded Operating System all right reserved.
//  
//  주담당자 : 아이구 (mymy49@nate.com) 2016.04.30 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#if defined(MAX32660)

#include <__cross_studio_io.h>

#include <config.h>
#include <drv/peripherals.h>

#if	defined(I2C1_ENABLE) && defined(MXC_I2C0)
static void setI2c0ClockEn(bool en)
{
	clock.peripheral.setI2c0En(en);
} 

drv::I2c i2c0(MXC_I2C0, setI2c0ClockEn, 0);
#endif

#if	defined(I2C1_ENABLE) && defined(MXC_I2C1)
static void setI2c1ClockEn(bool en)
{
	clock.peripheral.setI2c1En(en);
} 
#endif

namespace drv
{
	I2c::I2c(mxc_i2c_regs_t *peri, void (*clockFunc)(bool en), void (*nvicFunc)(bool en)) :  Drv(clockFunc, nvicFunc)
	{
		mPeri = peri;
	}

	bool I2c::init(unsigned char speed)
	{
		int clk = clock.getApbClkFreq();

		switch(speed)
		{
		case define::i2c::speed::STANDARD :
			clk /= 200000;
			break;
		case define::i2c::speed::FAST :
			clk /= 800000;
			break;
		}
	
		if(clk > 511)
			clk = 511;

		mPeri->clk_hi = clk;
		mPeri->clk_lo = clk;
		mPeri->ctrl |= MXC_F_I2C_CTRL_I2C_EN;
		mPeri->ctrl |= MXC_F_I2C_CTRL_MST;

		return true;
	}

	bool I2c::send(unsigned char addr, void *src, unsigned int size, unsigned int timeout)
	{
		mTimelapse.reset();
		mPeri->master_ctrl = MXC_F_I2C_MASTER_CTRL_START;

		while((mPeri->status & MXC_F_I2C_STATUS_STATUS) >> MXC_F_I2C_STATUS_STATUS_POS)
		{
			thread::yield();
			if(mTimelapse.getMsec() > timeout)
				return false;
		}
	
		mPeri->fifo = addr;

		for(int i=0;i<size;i++)
		{
	
		}

		return true;
	}

	bool I2c::receive(unsigned char addr, void *des, unsigned int size, unsigned int timeout)
	{
	
		return true;
	}

	void I2c::stop(void)
	{
	
	}
}

#endif
