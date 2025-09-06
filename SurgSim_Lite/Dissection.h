#pragma once
#include <Termin8or/geom/RC.h>
#include "AnatomyLineData.h"
#include "TexLineCharIdx.h"
#include "HealthStates.h"
#include "ParticleSystem.h"

namespace smoke
{
  t8x::ParticleHandler smoke_left(500), smoke_right(500);
}

void cut(HealthStates& health_states,
         std::vector<std::vector<AnatomyLineData>>& textures,
         const TexLineCharIdx& tlc_idx, float decay_rate, float dt,
         bool scissors)
{
  if (tlc_idx.line_idx != -1)
  {
    auto& decay = textures[tlc_idx.tex_idx][tlc_idx.line_idx].decay[tlc_idx.char_idx];
    decay += decay_rate * dt;
    if (decay > 1.f)
      decay = 1.f;
    if (scissors)
    {
      auto tex_type = static_cast<TextureType>(tlc_idx.tex_idx);
      if (tex_type == TextureType::ARTERY_TREE || tex_type == TextureType::DUCT_TREE)
        decay = 1.f; // Fully "cut".
      health_states.register_cut(tlc_idx.tex_idx);
    }
    else
      health_states.register_burn(tlc_idx.tex_idx); // Trigger diffuse xor profuse bleeding or gall leakage.
  }
};

void coagulate(HealthStates& health_states,
               std::vector<std::vector<AnatomyLineData>>& textures,
               const TexLineCharIdx& tlc_idx, float coag_rate, float dt)
{
  if (tlc_idx.line_idx != -1)
  {
    auto& coag = textures[tlc_idx.tex_idx][tlc_idx.line_idx].coag[tlc_idx.char_idx];
    coag += (coag_rate + math::lerp(rnd::rand(), 0.0f, 0.5f)) * dt;
    if (coag > 1.f)
    {
      coag = 1.f;
      cut(health_states, textures, tlc_idx, 0.05f, dt, false);
    }
    health_states.register_burn(tlc_idx.tex_idx);
  }
};


void update_burn(float dt, Key curr_key,
                 const RC& tcp_rc_left, const RC& tcp_rc_right,
                 ToolType tool_type_left, ToolType tool_type_right,
                 bool trg_tool_left, bool trg_tool_right,
                 std::vector<std::vector<AnatomyLineData>>& textures, // top layer at index 0.
                 HealthStates& health_states)
{
  int N_tex = static_cast<int>(textures.size());
  
  health_states.update();
  
  bool pedal_cut = curr_key == Key::Cut;
  bool pedal_coag = curr_key == Key::Coag;
  bool left_hook = tool_type_left == ToolType::Hook;
  bool right_hook = tool_type_right == ToolType::Hook;
  bool left_scissors = tool_type_left == ToolType::Scissors;
  bool right_scissors = tool_type_right == ToolType::Scissors;
  
  // Center, N, S, W, E, NW, NE, SW, SE
  auto fill_cluster = [](std::array<TexLineCharIdx, 9>& pixel_cluster, int tex_idx, const Pixel& px, int r, int c)
  {
    bool on_compatible_layer = pixel_cluster[0].tex_idx == tex_idx;
    if (pixel_cluster[0].tex_idx != -1 && tex_idx != -1)
    {
      TextureType center_tex = static_cast<TextureType>(pixel_cluster[0].tex_idx);
      TextureType test_tex = static_cast<TextureType>(tex_idx);
      if (center_tex == TextureType::HD_LIG)
      {
        if (test_tex == TextureType::ARTERY_TREE)
          on_compatible_layer = true;
        else if (test_tex == TextureType::DUCT_TREE)
          on_compatible_layer = true;
      }
      else if (center_tex == TextureType::ARTERY_TREE && test_tex == TextureType::HD_LIG)
        on_compatible_layer = true;
      else if (center_tex == TextureType::DUCT_TREE && test_tex == TextureType::HD_LIG)
        on_compatible_layer = true;
    }
    //else
    //  on_compatible_layer = true;
    
    int pc_idx = -1;
    if (pixel_cluster[0].line_idx == -1 && px.active && px.r == r && px.c == c)
      pc_idx = 0;
    else if (pixel_cluster[1].line_idx == -1 && on_compatible_layer && px.active && px.r == r - 1 && px.c == c)
      pc_idx = 1;
    else if (pixel_cluster[2].line_idx == -1 && on_compatible_layer && px.active && px.r == r + 1 && px.c == c)
      pc_idx = 2;
    else if (pixel_cluster[3].line_idx == -1 && on_compatible_layer && px.active && px.r == r && px.c == c - 1)
      pc_idx = 3;
    else if (pixel_cluster[4].line_idx == -1 && on_compatible_layer && px.active && px.r == r && px.c == c + 1)
      pc_idx = 4;
    else if (pixel_cluster[5].line_idx == -1 && on_compatible_layer && px.active && px.r == r - 1 && px.c == c - 1)
      pc_idx = 5;
    else if (pixel_cluster[6].line_idx == -1 && on_compatible_layer && px.active && px.r == r - 1 && px.c == c + 1)
      pc_idx = 6;
    else if (pixel_cluster[7].line_idx == -1 && on_compatible_layer && px.active && px.r == r + 1 && px.c == c - 1)
      pc_idx = 7;
    else if (pixel_cluster[8].line_idx == -1 && on_compatible_layer && px.active && px.r == r + 1 && px.c == c + 1)
      pc_idx = 8;
    if (pc_idx != -1)
      pixel_cluster[pc_idx] = { tex_idx, px.line_idx, px.char_idx };
  };
  
  auto fill_cluster_from_textures = [&](const RC& tcp_rc,
                                        int N_tex, std::vector<std::vector<AnatomyLineData>>& textures,
                                        std::array<TexLineCharIdx, 9>& pixel_cluster)
  {
    for (int pass = 0; pass < 2; ++pass)
      for (int tex_idx = 0; tex_idx < N_tex; ++tex_idx)
      {
        const auto& tex = textures[tex_idx];
        for (int iter = 0; iter < 2; ++iter)
          for (const auto& line : tex)
            for (const auto& px : line.get_pixels())
              fill_cluster(pixel_cluster, tex_idx, px, tcp_rc.r, tcp_rc.c);
      }
  };
  
  // ---
  
  // Find texture areas where to burn.
  std::array<TexLineCharIdx, 9> pixel_cluster_left;
  std::array<TexLineCharIdx, 9> pixel_cluster_right;
  if ((pedal_coag || pedal_cut))
  {
    if (left_hook)
      fill_cluster_from_textures(tcp_rc_left, N_tex, textures, pixel_cluster_left);
    else if (right_hook)
      fill_cluster_from_textures(tcp_rc_right, N_tex, textures, pixel_cluster_right);
  }
  else if (trg_tool_left && left_scissors)
    fill_cluster_from_textures(tcp_rc_left, N_tex, textures, pixel_cluster_left);
  else if (trg_tool_right && right_scissors)
    fill_cluster_from_textures(tcp_rc_right, N_tex, textures, pixel_cluster_right);
  
#define COAG_NEIGHBOURS
//#define CUT_NEIGHBOURS
  
  auto coag_pixels = [&](std::vector<std::vector<AnatomyLineData>>& textures,
                         const std::array<TexLineCharIdx, 9>& pixel_cluster,
                         float rate_center, float rate_nswe, float rate_diag,
                         float dt)
  {
    coagulate(health_states, textures, pixel_cluster[0], rate_center, dt);
#ifdef COAG_NEIGHBOURS
    for (int i = 1; i <= 4; ++i)
      coagulate(health_states, textures, pixel_cluster[i], rate_nswe, dt);
    for (int i = 5; i <= 8; ++i)
      coagulate(health_states, textures, pixel_cluster[i], rate_diag, dt);
#endif
  };
  
  auto cut_pixels = [&](std::vector<std::vector<AnatomyLineData>>& textures,
                        const std::array<TexLineCharIdx, 9>& pixel_cluster,
                        float rate_center, float rate_nswe, float rate_diag,
                        float dt, bool scissors)
  {
    cut(health_states, textures, pixel_cluster[0], rate_center, dt, scissors);
#ifdef CUT_NEIGHBOURS
    for (int i = 1; i <= 4; ++i)
      cut(health_states, textures, pixel_cluster[i], rate_nswe, dt, scissors);
    for (int i = 5; i <= 8; ++i)
      cut(health_states, textures, pixel_cluster[i], rate_diag, dt, scissors);
#endif
  };
  
  // Perform burning (or scissor cutting).
  if (pedal_coag)
  {
    if (left_hook)
      coag_pixels(textures, pixel_cluster_left, 1.f, 0.5f, 0.33f, dt);
    else if (right_hook)
      coag_pixels(textures, pixel_cluster_right, 1.f, 0.5f, 0.33f, dt);
  }
  else if (pedal_cut)
  {
    if (left_hook)
      cut_pixels(textures, pixel_cluster_left, 0.8f, 0.2f, 0.1f, dt, false);
    else if (right_hook)
      cut_pixels(textures, pixel_cluster_right, 0.8f, 0.2f, 0.1f, dt, false);
  }
  else if (trg_tool_left && left_scissors)
    cut_pixels(textures, pixel_cluster_left, 4.f, 2.f, 1.f, dt, true);
  else if (trg_tool_right && right_scissors)
    cut_pixels(textures, pixel_cluster_right, 4.f, 2.f, 1.f, dt, true);
}

template<int NR, int NC>
void generate_sparks(ScreenHandler<NR, NC>& sh, Key curr_key,
                     const RC& tcp_rc_left, const RC& tcp_rc_right,
                     ToolType tool_type_left, ToolType tool_type_right,
                     int anim_ctr)
{
  auto trig = curr_key == Key::Coag || curr_key == Key::Cut;
  auto trig_left = trig && tool_type_left == ToolType::Hook;
  auto trig_right = trig && tool_type_right == ToolType::Hook;
  int anim = anim_ctr % 6;
  auto bg_color = Color::White;
  switch (anim)
  {
    case 4:
    case 2:
    case 0: bg_color = Color::White; break;
    case 1: bg_color = Color::Yellow; break;
    case 3: bg_color = Color::Magenta; break;
    case 5: bg_color = Color::DarkYellow; break;
  }
  if (trig_left)
    sh.write_buffer(" ", tcp_rc_left.r, tcp_rc_left.c, Color::Default, bg_color);
  if (trig_right)
    sh.write_buffer(" ", tcp_rc_right.r, tcp_rc_right.c, Color::Default, bg_color);
}

template<int NR, int NC>
void generate_smoke(ScreenHandler<NR, NC>& sh, Key curr_key,
                    const RC& tcp_rc_left, const RC& tcp_rc_right,
                    ToolType tool_type_left, ToolType tool_type_right,
                    float dt, float time)
{
  auto trig = curr_key == Key::Coag || curr_key == Key::Cut;
  auto trig_left = trig && tool_type_left == ToolType::Hook;
  auto trig_right = trig && tool_type_right == ToolType::Hook;
  const float vel_r = 0.f, vel_c = 0.f, acc = -10.f, spread = 13.f, life_time = 6.f;
  const int cluster_size = 10;
  smoke::smoke_left.update(tcp_rc_left, trig_left, vel_r, vel_c, acc, spread, life_time, cluster_size, dt, time);
  smoke::smoke_right.update(tcp_rc_right, trig_right, vel_r, vel_c, acc, spread, life_time, cluster_size, dt, time);
  smoke::smoke_left.draw(sh, "&", Color::LightGray, Color::DarkGray, time);
  smoke::smoke_right.draw(sh, "&", Color::LightGray, Color::DarkGray, time);
}



void handle_clip_applying(Key curr_key,
                          const RC& tcp_rc_left, const RC& tcp_rc_right,
                          ToolType tool_type_left, ToolType tool_type_right,
                          bool trg_tool_left, bool trg_tool_right,
                          std::vector<std::vector<AnatomyLineData>>& textures,
                          HealthStates& health_states)
{
  const bool only_active = true;
  const bool orig_space = false;
  
  TexLineCharIdx tlc_idx_left, tlc_idx_right;
  
  if (tool_type_left == ToolType::ClipApplier
      && trg_tool_left)
  {
    tlc_idx_left = find_top_pixel(textures, tcp_rc_left, only_active, orig_space);
  }
  else if (curr_key == Key::RI_Trigger && tool_type_right == ToolType::ClipApplier
           && trg_tool_right)
  {
    tlc_idx_right = find_top_pixel(textures, tcp_rc_right, only_active, orig_space);
  }
  
  if (tlc_idx_left.tex_idx != -1)
  {
    textures[tlc_idx_left.tex_idx][tlc_idx_left.line_idx].clip[tlc_idx_left.char_idx] = 1;
    if (trg_tool_left)
      health_states.register_clip(tlc_idx_left.tex_idx);
  }
  else if (tlc_idx_right.tex_idx != -1)
  {
    textures[tlc_idx_right.tex_idx][tlc_idx_right.line_idx].clip[tlc_idx_right.char_idx] = 1;
    if (trg_tool_right)
      health_states.register_clip(tlc_idx_right.tex_idx);
  }
}
