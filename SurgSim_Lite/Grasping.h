#pragma once
#include "AnatomyLineData.h"
#include "InstrumentData.h"


namespace grasp
{

  RC locked_tcp_rc_left, locked_tcp_rc_right;
  int grasped_tex_idx_left = -1, grasped_tex_idx_right = -1;

  void handle_grasping(InstrumentData& instr_data_left, InstrumentData& instr_data_right,
                       ToolType tool_type_left, ToolType tool_type_right,
                       bool trg_tool_left, bool trg_tool_right,
                       int anim_idx_curr_left, int anim_idx_curr_right,
                       std::vector<std::vector<AnatomyLineData>>& all_textures)
  {
    auto update_grasping = [&all_textures]
                          (InstrumentData& instr_data, InstrumentSide side, bool using_grasper,
                           RC& tcp_rc_locked, int& grasped_tex_idx, bool trig, int anim_idx)
    {
      if (using_grasper && trig)
        tcp_rc_locked = instr_data.get_tcp();
      else if (using_grasper && anim_idx == 1 && tcp_rc_locked.r != -1)
      {
        const bool only_active = false; // Automatically excludes the artery and duct trees, which simplifies things.
        const bool orig_space = true;
        auto tlc_idx = find_top_pixel(all_textures, tcp_rc_locked, only_active, orig_space); // pre-calc?
        if (tlc_idx.tex_idx == -1)
        {
          tcp_rc_locked = {};
          return;
        }
        auto tex_type = static_cast<TextureType>(tlc_idx.tex_idx);
        int r_min = -1, r_max = +1, c_min = -2, c_max = +2;
        if (tex_type == TextureType::LIVER)
          c_min = 0;
        else if (tex_type == TextureType::HD_LIG)
        {
          r_max = 0; // #WARNING: Otherwise causes segmentation fault!
          r_min = 0;
        }
        else if (tex_type == TextureType::GALLBLADDER)
          r_min = 0; // #FIXME: Won't bite sometimes. Why? (Seems to happen when first grasping in thin air (seems to be fixed after resetting the lock position before the early exit due to pixel not found)).
        
        //if (side == InstrumentSide::Left)
        //  sh.write_buffer(std::to_string(tlc_idx.tex_idx), 1, 1, Color::White);
        
        auto diff = instr_data.get_tcp() - tcp_rc_locked;
        auto abs_diff = diff.abs();
        auto diff_clamped = diff.clamp(r_min, r_max, c_min, c_max);
        RC tcp_rc = instr_data.get_tcp();
        if (abs_diff.r > 2)
          tcp_rc.r = tcp_rc_locked.r + diff_clamped.r; //2*sign(diff.r);
        if (abs_diff.c > 2)
          tcp_rc.c = tcp_rc_locked.c + diff_clamped.c; //2*sign(diff.c);
        if (abs_diff.r > 2 || abs_diff.c > 2)
          instr_data.set(side, tcp_rc);

        if (grasped_tex_idx != -1)
          tlc_idx.tex_idx = grasped_tex_idx;
        if (tlc_idx.tex_idx != -1)
        {
          grasped_tex_idx = tlc_idx.tex_idx;
          auto& tex = all_textures[tlc_idx.tex_idx];
          for (auto& line : tex)
          {
            line.r = line.r_orig + diff_clamped.r;
            for (int ch_idx = 0; ch_idx < line.N; ++ch_idx)
              line.c[ch_idx] = line.c_orig[ch_idx] + diff_clamped.c;
          }
        }
      }
      else if (tcp_rc_locked.r != -1 && grasped_tex_idx != -1)
      {
        auto& tex = all_textures[grasped_tex_idx];
        for (auto& line : tex)
        {
          line.r = line.r_orig;
          for (int ch_idx = 0; ch_idx < line.N; ++ch_idx)
            line.c[ch_idx] = line.c_orig[ch_idx];
        }
        instr_data.set(side, tcp_rc_locked);
        tcp_rc_locked = {};
        grasped_tex_idx = -1;
      }
      
    };
    
    // Prioritize the left instrument over the right instrument.
    bool right_grasper = tool_type_right == ToolType::Grasper;
    update_grasping(instr_data_right, InstrumentSide::Right, right_grasper,
                    locked_tcp_rc_right, grasped_tex_idx_right, trg_tool_right, anim_idx_curr_right);
    bool left_grasper = tool_type_left == ToolType::Grasper;
    update_grasping(instr_data_left, InstrumentSide::Left, left_grasper,
                      locked_tcp_rc_left, grasped_tex_idx_left, trg_tool_left, anim_idx_curr_left);
  }

}
