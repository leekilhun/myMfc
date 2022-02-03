#pragma once

#include "def.h"

/*
 * jL_Registor.h
 *
 *  Created on: 2022. 1. 26.
 *      Author: gns2l
 */


enum class reg_bank {
  _0,
  _1,
  _2,
  _3,
  _max
};
#define REG_BANK(x)          static_cast<reg_bank>(x)



enum class reg_ {
  b0,
  b1,
  b2,
  b3,
  b4,
  b5,
  b6,
  b7,
  _max
};
#define REG_BIT(x)          static_cast<reg_>(x)



class _Regist {
  class _elem {
    bool m_bit[static_cast<uint8_t>(reg_::_max)];

    explicit _elem(const _elem& ref):m_bit(){}
    _elem& operator=(const _elem& ref) { return *this; }
  public:
    explicit _elem() :m_bit() {

    };
    virtual ~_elem() {};

  public:
    inline bool& operator[](const reg_ elem_no) {
      return m_bit[static_cast<int>(elem_no)];
    }

    inline void operator = (const uint8_t in_data) {
      uint8_t data = 0;
      for (uint8_t i=0; i < 8; i++) {
        data = in_data;
        data = data >> i;
        data = 0xfe | data;
        data = ~(data);
        m_bit[i] = !data; //!(~(0xfe | (data >> i)));
      }
    }

    inline uint8_t get() {
      uint8_t data;
      data = (int)m_bit[0] | ((int)m_bit[1] * 2) | ((int)m_bit[2] * 4) | ((int)m_bit[3] * 8)
        | ((int)m_bit[4] * 16) | ((int)m_bit[5] * 32) | ((int)m_bit[6] * 64 | ((int)m_bit[7] * 128));
      return data;
    }

  };

  _elem m_byte[static_cast<uint8_t>(reg_bank::_max)];
  uint8_t m_data[static_cast<uint8_t>(reg_bank::_max)];

  explicit _Regist(const _Regist& ref) :m_byte(), m_data() {}
  _Regist& operator=(const _Regist ref) { return *this; }
public:
  _Regist() :m_byte(), m_data() { };
  virtual ~_Regist() {};

public:
  inline void ClearAll() {
      Clear(reg_bank::_0);
      Clear(reg_bank::_1);
      Clear(reg_bank::_2);
      Clear(reg_bank::_3);
  }

  inline void Clear(reg_bank bank) {
    m_byte[static_cast<uint8_t>(bank)] = 0;
  }

  inline uint8_t SetOn(reg_bank bank, reg_ reg_no) {
    m_byte[static_cast<uint8_t>(bank)][reg_no] = true;
    return m_byte[static_cast<uint8_t>(bank)].get();
  }
  inline uint8_t SetOff(reg_bank bank, reg_ reg_no) {
    m_byte[static_cast<uint8_t>(bank)][reg_no] = false;
    return m_byte[static_cast<uint8_t>(bank)].get();
  }

  inline bool Get(reg_bank bank, reg_ bit) {
    return m_byte[static_cast<uint8_t>(bank)][bit];
  }

  inline uint8_t* Out() {
    m_data[0] = m_byte[0].get();
    m_data[1] = m_byte[1].get();
    m_data[2] = m_byte[2].get();
    m_data[3] = m_byte[3].get();

    return m_data;
  }

  inline _elem& operator[](reg_bank bank) {
    return m_byte[static_cast<int>(bank)];
  }

  inline uint8_t operator[](const int reg) {
    return m_byte[reg].get();
  }
};


