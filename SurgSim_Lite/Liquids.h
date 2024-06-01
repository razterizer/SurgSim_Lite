//
//  Liquids.h
//  Surgsim_Lite
//
//  Created by Rasmus Anthin on 2022-10-22.
//

#pragma once
#include "TubeTopology.h"
#include "LiquidVolumes.h"
#include "ParticleSystem.h"
#include <Termin8or/ColorGradient.h>
#include <Termin8or/Screen.h>
#include <optional>

namespace liquids
{

  std::vector<RC> reg_leakage_pts_artery_tree, reg_leakage_pts_duct_tree;

  struct Reservoir
  {
    Reservoir(float init_vol = 0.f)
      : active(init_vol > 0.f), vol(init_vol), vol_orig(init_vol) {}
    bool active = false;
    float vol = 0.f;
    float vol_orig = 0.f;
    float calc_vol_t() const { return active ? vol / vol_orig : 1.f; }
    void operator-=(float droplet_vol)
    {
      if (!active)
        return;
      vol -= droplet_vol;
      if (vol < 0)
        vol = 0.f;
    }
  };
  Reservoir artery_reservoir_vol;
  Reservoir duct_reservoir_vol(150.f);

  struct LiquidFlow
  {
    float blood_flow = 0.f;
    float bile_flow = 0.f;
    float water_flow = 0.f;
    
    void reset()
    {
      blood_flow = 0.f;
      bile_flow = 0.f;
      water_flow = 0.f;
    }
    void set(LiquidType type, float flow)
    {
      switch(type)
      {
        case LiquidType::Blood: blood_flow = flow; break;
        case LiquidType::Bile: bile_flow = flow; break;
        case LiquidType::Water: water_flow = flow; break;
        case LiquidType::NUM_ITEMS: break;
      }
    }
  };


  ParticleFluidHandler rinsing_droplets_left(500), rinsing_droplets_right(500);
  ParticleFluidHandler artery_droplets(500), duct_droplets(500);


  ColorGradient fluid_pool_mixing_gradient_blood_bile_fg // blood to bile
  {
    {
      { 0.f, Text::Color::Red },
      { 0.5f, Text::Color::LightGray },
      { 1.f, Text::Color::Yellow }
    }
  };
  ColorGradient fluid_pool_mixing_gradient_blood_bile_bg // blood to bile
  {
    {
      { 0.f, Text::Color::DarkRed },
      { 0.5f, Text::Color::DarkGray },
      { 1.f, Text::Color::DarkYellow }
    }
  };
  ColorGradient fluid_pool_mixing_gradient_water_blood_fg
  {
    {
      { 0.f, Text::Color::Blue },
      { 0.5f, Text::Color::LightGray },
      { 1.f, Text::Color::Red }
    }
  };
  ColorGradient fluid_pool_mixing_gradient_water_blood_bg
  {
    {
      { 0.f, Text::Color::DarkBlue },
      { 0.5f, Text::Color::DarkGray },
      { 1.f, Text::Color::DarkRed }
    }
  };
  ColorGradient fluid_pool_mixing_gradient_bile_water_fg
  {
    {
      { 0.f, Text::Color::Yellow },
      { 0.5f, Text::Color::LightGray },
      { 1.f, Text::Color::Blue }
    }
  };
  ColorGradient fluid_pool_mixing_gradient_bile_water_bg
  {
    {
      { 0.f, Text::Color::DarkYellow },
      { 0.5f, Text::Color::DarkGray },
      { 1.f, Text::Color::DarkBlue }
    }
  };

  Text::Color get_liquid_fg_color(LiquidVolumes volumes)
  {
    float t_blood = volumes.t_blood();
    float t_bile = volumes.t_bile();
    float t_water = volumes.t_water();
    if (t_blood >= t_water && t_bile >= t_water)
      return fluid_pool_mixing_gradient_blood_bile_fg(volumes.t_blood_bile());
    else if (t_bile >= t_blood && t_water >= t_blood)
      return fluid_pool_mixing_gradient_bile_water_fg(volumes.t_bile_water());
    else if (t_blood >= t_bile && t_water >= t_bile)
      return fluid_pool_mixing_gradient_water_blood_fg(volumes.t_water_blood());
    return Text::Color::Default;
  }

  Text::Color get_liquid_bg_color(LiquidVolumes volumes)
  {
    float t_blood = volumes.t_blood();
    float t_bile = volumes.t_bile();
    float t_water = volumes.t_water();
    if (t_blood >= t_water && t_bile >= t_water)
      return fluid_pool_mixing_gradient_blood_bile_bg(volumes.t_blood_bile());
    else if (t_bile >= t_blood && t_water >= t_blood)
      return fluid_pool_mixing_gradient_bile_water_bg(volumes.t_bile_water());
    else if (t_blood >= t_bile && t_water >= t_bile)
      return fluid_pool_mixing_gradient_water_blood_bg(volumes.t_water_blood());
    return Text::Color::Default;
  }

  template<int NR, int NC>
  void update_profuse_liquids(SpriteHandler<NR, NC>& sh,
                              Key curr_key,
                              const RC& tcp_rc_left, const RC& tcp_rc_right,
                              ToolType tool_type_left, ToolType tool_type_right,
                              std::vector<std::vector<AnatomyLineData>>& all_textures,
                              LiquidVolumes& liquid_volumes, LiquidFlow& liquid_flow,
                              std::vector<RC>& fluid_sources,
                              float time, float dt)
  {
    liquid_flow.reset();
    
    // Detect bleeding.
    // Conditions:
    // cut (diathermy / scissors) without clips.
    std::vector<tube_topology::TopologyElem> topoelem_artery_tree, topoelem_duct_tree;
    for (auto tex_type : { TextureType::ARTERY_TREE, TextureType::DUCT_TREE })
    {
      const auto& texture = all_textures[static_cast<size_t>(tex_type)];
      for (const auto& line : texture)
        for (const auto& px : line.get_pixels())
        {
          if (!px.active)
          {
            if (tex_type == TextureType::ARTERY_TREE)
            {
              int topo_idx = tube_topology::find_tube_topology_pixel({ px.r, px.c }, TextureType::ARTERY_TREE);
              topoelem_artery_tree.emplace_back(tube_topology::artery_tree_topology[topo_idx]);
            }
            else if (tex_type == TextureType::DUCT_TREE)
            {
              int topo_idx = tube_topology::find_tube_topology_pixel({ px.r, px.c }, TextureType::DUCT_TREE);
              topoelem_duct_tree.emplace_back(tube_topology::duct_tree_topology[topo_idx]);
            }
          }
        }
    }
  
    auto find_leakage_pt = [](const std::vector<std::vector<AnatomyLineData>>& all_textures,
                            const std::vector<tube_topology::TopologyElem>& topoelem,
                            TextureType tex_type)
    {
      RC leakage_pt;
      for (const auto& te : topoelem)
      {
        if (te.next == nullptr)
          return te.curr;
        else
        {
          auto* next_node = te.next;
          TexLineCharIdx tlc_next;
          int clip_next = 0;
          float coag_next = 0.f;
          do
          {
            tlc_next = find_pixel(all_textures, tex_type, next_node->curr); // te.next->curr.
            clip_next = all_textures[tlc_next.tex_idx][tlc_next.line_idx].clip[tlc_next.char_idx];
            coag_next = all_textures[tlc_next.tex_idx][tlc_next.line_idx].coag[tlc_next.char_idx];
            next_node = next_node->next;
          } while (next_node != nullptr && clip_next == 0 && coag_next < coag_sufficient);
          if (tlc_next.tex_idx != -1)
          {
            if (clip_next == 0 && coag_next < coag_sufficient) // No clip or enough coag.
              if (te.curr.r > leakage_pt.r)
                leakage_pt = te.curr;
          }
        }
      }
      return leakage_pt;
    };
  
    // Momentary detection of leaking pixels.
    auto new_leakage_pt_artery_tree = find_leakage_pt(all_textures, topoelem_artery_tree, TextureType::ARTERY_TREE);
    auto new_leakage_pt_duct_tree = find_leakage_pt(all_textures, topoelem_duct_tree, TextureType::DUCT_TREE);
    fluid_sources.emplace_back(new_leakage_pt_artery_tree);
    fluid_sources.emplace_back(new_leakage_pt_duct_tree);
    
    // Update of cached leaking pixels with time stamps.
    auto register_leaks = [](std::vector<RC>& registered_leakage_pts,
                             const RC& new_leakage_pt,
                             float time)
    {
      // Add leaks.
      if (new_leakage_pt.r != -1)
      {
        auto it = std::find_if(registered_leakage_pts.begin(), registered_leakage_pts.end(),
                               [&new_leakage_pt](const auto& reg_pt)
                               { return new_leakage_pt == reg_pt; });
        if (it == registered_leakage_pts.end())
          registered_leakage_pts.emplace_back(new_leakage_pt);
      }
      // Remove leaks.
      for (const auto& reg_pt : registered_leakage_pts)
      {
        /*
        auto it_new = std::find_if(new_leakage_pts.begin(), new_leakage_pts.end(),
                               [&leakage_pt](const auto& leak_pt) { return leak_pt.r == leakage_pt.pos.r && leak_pt.c == leakage_pt.pos.c; });
        if (it_new == new_leakage_pts.end())
         */
        if (reg_pt != new_leakage_pt)
        {
          auto it = std::remove(registered_leakage_pts.begin(), registered_leakage_pts.end(), reg_pt);
          registered_leakage_pts.erase(it, registered_leakage_pts.end());
        }
      }
    };
    
    register_leaks(reg_leakage_pts_artery_tree, new_leakage_pt_artery_tree, time);
    register_leaks(reg_leakage_pts_duct_tree, new_leakage_pt_duct_tree, time);
    
    auto update_dripping = [&liquid_volumes, &liquid_flow, dt, time, &sh]
                           (std::vector<RC>& registered_leakage_pts,
                            ParticleFluidHandler& droplets,
                            float speed, float gravity_acc,
                            float spread, float life_time, int cluster_size,
                            LiquidType liquid_type, float droplet_vol, Reservoir& reservoir_vol)
    {
      float dir_x = 0.f;
      float dir_y = 0.f;
      switch (liquid_type)
      {
        case LiquidType::Blood:
          dir_x = 1.f;
          dir_y = -20.f;
          break;
        case LiquidType::Bile:
          dir_x = 0.5f;
          dir_y = 2.f;
          break;
        default:
          break;
      }
      math::normalize(dir_x, dir_y);
      for (auto& leakage_pt : registered_leakage_pts)
      {
        float bernoulli_factor = math::lerp(reservoir_vol.calc_vol_t(), 0.01f, 1.f);
        droplets.update(leakage_pt, true,
                        dir_x * speed, dir_y * speed, gravity_acc * bernoulli_factor, spread * bernoulli_factor, life_time, cluster_size,
                        liquid_volumes, liquid_type, droplet_vol, dt, time);
        liquid_flow.set(liquid_type, speed * spread);
        reservoir_vol -= droplet_vol;
      }
      droplets.update({ -1, -1 }, false, 0.f, 0.f, 0.f, 0.f, 0,
                      life_time, liquid_volumes, liquid_type, droplet_vol, dt, time);
      droplets.set_num_active_particles(reservoir_vol.calc_vol_t());
    };
    const float artery_speed = 31.f; //26.f;
    const float artery_g = 20.f; //14.f;
    const float artery_spread = 3.f; // 4.f
    const float artery_life_time = 4.f;
    const int artery_cluster_size = 1;
    const float artery_droplet_vol = 1.f;
    const float duct_speed = 0.2f;
    const float duct_g = 14.f;
    const float duct_spread = 4.f;
    const float duct_life_time = 5.f; //4.f;
    const int duct_cluster_size = 1;
    const float duct_droplet_vol = 0.5f;
    update_dripping(reg_leakage_pts_artery_tree, artery_droplets,
                    artery_speed, artery_g, artery_spread, artery_life_time, artery_cluster_size,
                    LiquidType::Blood, artery_droplet_vol, artery_reservoir_vol);
    update_dripping(reg_leakage_pts_duct_tree, duct_droplets,
                    duct_speed, duct_g, duct_spread, duct_life_time, duct_cluster_size,
                    LiquidType::Bile, duct_droplet_vol, duct_reservoir_vol);
    
    
    // Suction / Rinsing
    
    const float screen_width = 80.f;
    
    auto update_suction = [&liquid_volumes, screen_width]
                          (const RC& tcp, float vol_rate)
    {
      float pool_height = liquid_volumes.pool_volume / screen_width;
      int r = std::ceil(29 - pool_height);
      if (tcp.r >= r)
        liquid_volumes -= vol_rate;
    };
    
    auto update_rinsing = [&liquid_volumes, &liquid_flow, screen_width, dt, time, &all_textures]
                          (const RC& tcp, InstrumentSide side,
                           bool rinsing,
                           ParticleFluidHandler& droplets,
                           float speed, float gravity_acc,
                           float spread, float life_time, int cluster_size,
                           float droplet_vol, float cleaning_rate)
    {
      float dir_x = 0.f;
      float dir_y = 0.f;
      switch (side)
      {
        case InstrumentSide::Left:
          dir_x = tcp.c;
          dir_y = tcp.r - 29;
          break;
        case InstrumentSide::Right:
          dir_x = tcp.c - 79;
          dir_y = tcp.r - 29;
          break;
        case InstrumentSide::NUM_ITEMS:
          break;
      }
      math::normalize(dir_x, dir_y);
      droplets.update(tcp, rinsing,
                      speed*dir_x, speed*dir_y, gravity_acc,
                      spread, life_time, cluster_size,
                      liquid_volumes, LiquidType::Water, droplet_vol,
                      dt, time);
      if (rinsing)
        liquid_flow.set(LiquidType::Water, speed * spread);
      
      // Clean up charred tissue.
      for (const auto& drop : droplets.particle_stream)
      {
        auto drop_curr_rc = RC { static_cast<int>(std::round(drop.pos_y)), static_cast<int>(std::round(drop.pos_x)) };
        const bool only_active = true;
        const bool orig_space = false;
        TexLineCharIdx tlc_idx = find_top_pixel(all_textures, drop_curr_rc, only_active, orig_space);
        if (tlc_idx.tex_idx != -1)
        {
          auto& px_coag = all_textures[tlc_idx.tex_idx][tlc_idx.line_idx].coag[tlc_idx.char_idx];
          if (px_coag > coag_sufficient)
          {
            px_coag -= cleaning_rate;
            if (px_coag < coag_sufficient)
              px_coag = coag_sufficient;
          }
        }
      }
    };
    
    bool using_suctionrinsing_left = tool_type_left == ToolType::SuctionRinsing;
    bool using_suctionrinsing_right = tool_type_right == ToolType::SuctionRinsing;
    bool left_suction = curr_key == Key::LI_Trigger;
    bool right_suction = curr_key == Key::RI_Trigger;
    bool left_rinsing = curr_key == Key::LI_Trigger2;
    bool right_rinsing = curr_key == Key::RI_Trigger2;
    const float suction_vol_rate = 10.f;
    const float rinse_speed = 20.f; //27.f;//16.f;
    const float rinse_g = 14.f; //10.f;
    const float rinse_spread = 4.f;
    const float rinse_life_time = 4.f;
    const int rinse_cluster_size = 2;
    const float rinse_droplet_vol = 1.f;
    const float rinse_cleaning_rate = 0.03f;
    if (using_suctionrinsing_left)
    {
      if (left_suction)
        update_suction(tcp_rc_left, suction_vol_rate);
    }
    if (using_suctionrinsing_right)
    {
      if (right_suction)
        update_suction(tcp_rc_right, suction_vol_rate);
    }
    update_rinsing(tcp_rc_left, InstrumentSide::Left,
                   using_suctionrinsing_left && left_rinsing,
                   rinsing_droplets_left,
                   rinse_speed, rinse_g, rinse_spread, rinse_life_time, rinse_cluster_size,
                   rinse_droplet_vol, rinse_cleaning_rate);
    update_rinsing(tcp_rc_right, InstrumentSide::Right,
                   using_suctionrinsing_right && right_rinsing,
                   rinsing_droplets_right,
                   rinse_speed, rinse_g, rinse_spread, rinse_life_time, rinse_cluster_size,
                   rinse_droplet_vol, rinse_cleaning_rate);
    liquid_volumes.update();
    
    // Drawing
    
    float pool_height = liquid_volumes.pool_volume / screen_width;
    for (int r = 29; r >= 0; --r)
    {
      // Draw fluid pool.
      auto fg_color = get_liquid_fg_color(liquid_volumes);
      auto bg_color = get_liquid_bg_color(liquid_volumes);
      float y = 29 - r;
      if (y <= pool_height)
        sh.write_buffer(str::rep_char('~', 80), r, 0, fg_color, bg_color);
    }
    
    artery_droplets.draw(sh, "v", Text::Color::Red, Text::Color::DarkRed, time);
    duct_droplets.draw(sh, "v", Text::Color::Yellow, Text::Color::DarkYellow, time);
    
    rinsing_droplets_left.draw(sh, "v", Text::Color::Blue, Text::Color::DarkBlue, time);
    rinsing_droplets_right.draw(sh, "v", Text::Color::Blue, Text::Color::DarkBlue, time);
  }

}
