#pragma once
#include "Enums.h"
#include <Core/Math.h>


class InstrumentData
{
  float pivot_x = 0.f;
  float pivot_y = 0.f;
  float end_x = 0.f;
  float end_y = 0.f;
  RC tcp_rc;
  InstrumentSide instr_side = InstrumentSide::Left;
  const float pix_ar = 0.f;
  const float shaft_len = 0.f;
  float shaft_z = 0.f;
  float ang_rad = 0.f;
  
  void clamp_angle()
  {
    float ang_min_rad = instr_side == InstrumentSide::Left ? 0.f : math::deg2rad(-90.f);
    float ang_max_rad = instr_side == InstrumentSide::Left ? math::deg2rad(90.f) : 0.f;
    ang_rad = math::clamp<float>(ang_rad, ang_min_rad, ang_max_rad);
  }
  
  void clamp_insertion()
  {
    shaft_z = math::clamp<float>(shaft_z, 0, shaft_len);
  }

public:
  InstrumentData(InstrumentSide side, float shaft_length, float shaft_z_len, float angle_rad, const float px_aspect)
    : instr_side(side), shaft_len(shaft_length), shaft_z(shaft_z_len), ang_rad(angle_rad), pix_ar(px_aspect)
  {
    update();
  }
  
  void update()
  {
    clamp_angle();
    clamp_insertion();
    pivot_x = instr_side == InstrumentSide::Left ? 0.f : 80.f;
    pivot_y = 30;
    float sgn = instr_side == InstrumentSide::Left ? +1.f : -1.f;
    end_x = pivot_x + sgn * std::round(shaft_z * std::cos(ang_rad));
    end_y = pivot_y - sgn * std::round(shaft_z * std::sin(ang_rad) / pix_ar);
    int r = static_cast<int>(end_y - sgn * std::round(std::sin(ang_rad)));
    int c = static_cast<int>(end_x + sgn * std::round(std::cos(ang_rad)));
    tcp_rc = { r, c };
  }
  
  void set(InstrumentSide side, const RC& rc)
  {
    float dir_x = 0.f;
    float dir_y = 0.f;
    
    pivot_x = side == InstrumentSide::Left ? 0.f : 80.f;
    pivot_y = 30;
    float sgn = side == InstrumentSide::Left ? +1.f : -1.f;
    tcp_rc = rc;
    
    // Calculate approximate shaft insertion from tcp.
    dir_x = (rc.c-sgn*1) - pivot_x;
    dir_y = ((rc.r+1) - pivot_y) * pix_ar;
    shaft_z = math::length<float>(dir_x, dir_y);
    
    // Calculate preliminary approximate shaft angle from tcp.
#if true
    math::normalize(dir_x, dir_y);
    ang_rad = std::atan2(-sgn*dir_y, sgn*dir_x);
#endif
    
    // Calculate shaft end pos from tcp and preliminary angle.
    end_y = rc.r + sgn * std::ceil(std::sin(ang_rad) / pix_ar);
    end_x = rc.c - sgn * std::round(std::cos(ang_rad));
    
    // Recalculate more accurate angle from shaft end pos and shaft pivot pos.
#if true
    dir_x = end_x - pivot_x;
    dir_y = (end_y - pivot_y) * pix_ar;
    math::normalize(dir_x, dir_y);
    ang_rad = std::atan2(-sgn*dir_y, sgn*dir_x);
#endif
  }
  
  const RC& get_tcp() const { return tcp_rc; }
  
  void step_angle(float ang_step_rad) { ang_rad += ang_step_rad; }
  void step_insertion(float z_step) { shaft_z += z_step; }
  
  float get_pivot_x() const { return pivot_x; }
  float get_pivot_y() const { return pivot_y; }
  float get_end_x() const { return end_x; }
  float get_end_y() const { return end_y; }
};
