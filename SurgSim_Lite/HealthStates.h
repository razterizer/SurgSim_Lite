#pragma once
#include "Enums.h"
#include "AnatomyLineData.h"
#include "Liquids.h"
#include <Core/Math.h>
#include <Core/FlankDetector.h>
#include <Core/OneShot.h>
#include <Termin8or/MessageHandler.h>

class HealthStates
{
  FlankDetector<bool> burn_liver;
  FlankDetector<bool> burn_gallbladder;

  FlankDetector<bool> cut_liver;
  FlankDetector<bool> cut_gallbladder;
  
  FlankDetector<bool> clip_liver;
  FlankDetector<bool> clip_gallbladder;
  
  FlankDetector<bool> bleeding_critical;
  
  ///

  FlankDetector<bool> cystic_artery_properly_visible;
  FlankDetector<bool> cystic_duct_properly_visible;

  std::vector<RC> cystic_artery_rc // arteria cystica
  {
    { 19, 43 },
    { 20, 43 },
    { 21, 43 },
  };
  std::vector<RC> cystic_duct_rc // ductus cysticus
  {
    { 19, 45 },
    { 20, 45 },
    { 21, 45 },
  };
  
  std::vector<RC> common_hepatic_artery_rc // arteria hepatica propria
  {
    { 22, 40 },
    { 22, 41 },
    { 22, 42 },
    { 22, 43 },
  };
  std::vector<RC> common_hepatic_bile_duct_rc // ductus hepaticus
  {
    { 22, 41 },
    { 22, 42 },
    { 22, 43 },
    { 22, 44 },
    { 22, 45 },
  };
  
  std::vector<RC> common_artery_rc // arteria gastroduodenalis
  {
    { 23, 43 },
    { 24, 43 },
  };
  std::vector<RC> common_bile_duct_rc // ductus choledochus
  {
    { 23, 45 },
    { 24, 45 },
  };
  
  ///
  
  liquids::LiquidVolumes liquid_volumes;
  liquids::LiquidFlow liquid_flow;
  float critical_blood_volume = 0.f;
  
  FlankDetector<bool> cystic_artery_correctly_divided;
  FlankDetector<bool> cystic_duct_correctly_divided;
  
  FlankDetector<bool> artery_tree_is_leaking;
  FlankDetector<bool> duct_tree_is_leaking;
  
  bool has_flowed_blood = false;
  bool has_flowed_bile = false;
  bool has_flowed_water = false;
  
  bool has_drained_blood = false;
  bool has_drained_bile = false;
  bool has_drained_water = false;
  
  std::vector<RC> fluid_sources;
  
  FlankDetector<bool> damaged_duct_root;
  FlankDetector<bool> leaky_arteria_hepatica;
  FlankDetector<bool> leaky_ductus_hepaticus;
  FlankDetector<bool> leaky_arteria_gastroduodenalis;
  FlankDetector<bool> leaky_ductus_choledochus;
  
  OneShot pool_msg_displayed;
  
public:
  void register_burn(int tex_idx)
  {
    if (tex_idx == static_cast<int>(TextureType::LIVER))
      burn_liver.set(true);
    if (tex_idx == static_cast<int>(TextureType::GALLBLADDER))
      burn_gallbladder.set(true);
  }
  
  void register_cut(int tex_idx)
  {
    if (tex_idx == static_cast<int>(TextureType::LIVER))
      cut_liver.set(true);
    if (tex_idx == static_cast<int>(TextureType::GALLBLADDER))
      cut_gallbladder.set(true);
  }
  
  void register_clip(int tex_idx)
  {
    if (tex_idx == static_cast<int>(TextureType::LIVER))
      clip_liver.set(true);
    if (tex_idx == static_cast<int>(TextureType::GALLBLADDER))
      clip_gallbladder.set(true);
  }
  
  void register_fluids(liquids::LiquidVolumes liq_vols, liquids::LiquidFlow liq_flow, const std::vector<RC>& sources)
  {
    liquid_volumes = liq_vols;
    liquid_flow = liq_flow;
    fluid_sources = sources;
  }
  
  void set_critical_blood_vol(float crit_blood_vol)
  {
    critical_blood_volume = crit_blood_vol;
  }
  
  void check_visibility(const std::vector<std::vector<AnatomyLineData>>& all_textures, TextureType tex_type, int& score)
  {
    cystic_artery_properly_visible.update(true);
    for (const auto& rc : cystic_artery_rc)
    {
      auto tlc_idx = find_pixel(all_textures, TextureType::HD_LIG, rc);
      if (all_textures[tlc_idx.tex_idx][tlc_idx.line_idx].get_pixels()[tlc_idx.char_idx].active)
        cystic_artery_properly_visible.set(false);
    }
    if (cystic_artery_properly_visible.pos_flank())
      score += 500;
    
    cystic_duct_properly_visible.update(true);
    for (const auto& rc : cystic_duct_rc)
    {
      auto tlc_idx = find_pixel(all_textures, TextureType::HD_LIG, rc);
      if (all_textures[tlc_idx.tex_idx][tlc_idx.line_idx].get_pixels()[tlc_idx.char_idx].active)
        cystic_duct_properly_visible.set(false);
    }
    if (cystic_duct_properly_visible.pos_flank())
      score += 500;
  }
  
  void check_correct_artery_division(const std::vector<std::vector<AnatomyLineData>>& all_textures, int& score)
  {
    // #FIXME
    cystic_artery_correctly_divided.update(true);
    
    auto rc0 = cystic_artery_rc[0];
    auto tlc_idx0 = find_pixel(all_textures, TextureType::ARTERY_TREE, rc0);
    if (all_textures[tlc_idx0.tex_idx][tlc_idx0.line_idx].clip[tlc_idx0.char_idx] != 1)
      cystic_artery_correctly_divided.set(false);
    
    auto rc1 = cystic_artery_rc[1];
    auto tlc_idx1 = find_pixel(all_textures, TextureType::ARTERY_TREE, rc1);
    if (all_textures[tlc_idx1.tex_idx][tlc_idx1.line_idx].get_pixels()[tlc_idx1.char_idx].active != false)
      cystic_artery_correctly_divided.set(false);
    
    auto rc2 = cystic_artery_rc[2];
    auto tlc_idx2 = find_pixel(all_textures, TextureType::ARTERY_TREE, rc2);
    if (all_textures[tlc_idx2.tex_idx][tlc_idx2.line_idx].clip[tlc_idx2.char_idx] != 1)
      cystic_artery_correctly_divided.set(false);
    
    if (cystic_artery_correctly_divided.pos_flank())
    {
      if (has_flowed_blood)
        score += 300;
      else
        score += 1000;
    }
  }
  
  void check_correct_duct_division(const std::vector<std::vector<AnatomyLineData>>& all_textures, int& score)
  {
    // #FIXME
    cystic_duct_correctly_divided.update(true);
    
    auto rc0 = cystic_duct_rc[0];
    auto tlc_idx0 = find_pixel(all_textures, TextureType::DUCT_TREE, rc0);
    if (all_textures[tlc_idx0.tex_idx][tlc_idx0.line_idx].clip[tlc_idx0.char_idx] != 1)
      cystic_duct_correctly_divided.set(false);
    
    auto rc1 = cystic_duct_rc[1];
    auto tlc_idx1 = find_pixel(all_textures, TextureType::DUCT_TREE, rc1);
    if (all_textures[tlc_idx1.tex_idx][tlc_idx1.line_idx].get_pixels()[tlc_idx1.char_idx].active != false)
      cystic_duct_correctly_divided.set(false);
    
    auto rc2 = cystic_duct_rc[2];
    auto tlc_idx2 = find_pixel(all_textures, TextureType::DUCT_TREE, rc2);
    if (all_textures[tlc_idx2.tex_idx][tlc_idx2.line_idx].clip[tlc_idx2.char_idx] != 1)
      cystic_duct_correctly_divided.set(false);
    
    if (cystic_duct_correctly_divided.pos_flank())
    {
      if (has_flowed_bile)
        score += 300;
      else
        score += 1000;
    }
  }
  
  void check_liquid_pool_empty(int& score)
  {
    if (cystic_artery_divided() && !has_drained_blood)
    {
      if (has_flowed_blood)
      {
        if (liquid_volumes.blood_volume < 1.f)
        {
          score += 200;
          has_drained_blood = true;
        }
      }
      else
      {
        score += 500;
        has_drained_blood = true;
      }
    }
    
    if (cystic_duct_divided() && !has_drained_bile)
    {
      if (has_flowed_bile)
      {
        if (liquid_volumes.bile_volume < 1.f)
        {
          score += 200;
          has_drained_bile = true;
        }
      }
      else
      {
        score += 500;
        has_drained_bile = true;
      }
    }
    
    if (!has_drained_water && has_flowed_water && liquid_volumes.water_volume < 1.f)
    {
      score += 100;
      has_drained_water = true;
    }
  }
  
  void update()
  {
    burn_liver.update(false);
    burn_gallbladder.update(false);
    
    cut_liver.update(false);
    cut_gallbladder.update(false);
    
    clip_liver.update(false);
    clip_gallbladder.update(false);
    
    bleeding_critical.update(liquid_volumes.blood_volume >= critical_blood_volume);
    
    artery_tree_is_leaking.update(liquid_flow.blood_flow > 0.f);
    duct_tree_is_leaking.update(liquid_flow.bile_flow > 0.f);
    
    if (liquid_flow.blood_flow > 2.f)
      has_flowed_blood = true;
    if (liquid_flow.bile_flow > 2.f)
      has_flowed_bile = true;
    if (liquid_flow.water_flow > 2.f)
      has_flowed_water = true;
    
    damaged_duct_root.update(std::find(fluid_sources.begin(), fluid_sources.end(), cystic_duct_rc[0]) != fluid_sources.end());
    
    auto find_leak = [&](const std::vector<RC>& tube_tree_rc)
    {
      bool found_leaky_pos = false;
      for (const auto& src : fluid_sources)
        if (std::find(tube_tree_rc.begin(), tube_tree_rc.end(), src) != tube_tree_rc.end())
          found_leaky_pos = true;
      return found_leaky_pos;
    };
    
    leaky_arteria_hepatica.update(find_leak(common_hepatic_artery_rc));
    leaky_ductus_hepaticus.update(find_leak(common_hepatic_bile_duct_rc));
    leaky_arteria_gastroduodenalis.update(find_leak(common_artery_rc));
    leaky_ductus_choledochus.update(find_leak(common_bile_duct_rc));
    
    if (liquid_volumes.pool_volume < 1.f)
      pool_msg_displayed.reset();
  }
  
  bool burned_transient_liver() const { return burn_liver.pos_flank(); }
  bool burned_transient_gallbladder() const { return burn_gallbladder.pos_flank(); }
  bool cut_transient_liver() const { return cut_liver.pos_flank(); }
  bool cut_transient_gallbladder() const { return cut_gallbladder.pos_flank(); }
  bool clipped_transient_liver() const { return clip_liver.pos_flank(); }
  bool clipped_transient_gallbladder() const { return clip_gallbladder.pos_flank(); }
  bool bleeding_critical_trans() const { return bleeding_critical.pos_flank(); }
  float get_blood_vol() const { return liquid_volumes.blood_volume; }
  float get_crit_blood_vol() const { return critical_blood_volume; }
  bool cystic_artery_divided() const { return cystic_artery_correctly_divided.curr(); }
  bool cystic_duct_divided() const { return cystic_duct_correctly_divided.curr(); }
  bool artery_tree_leaking_trans() const { return artery_tree_is_leaking.pos_flank(); }
  bool duct_tree_leaking_trans() const { return duct_tree_is_leaking.pos_flank(); }
  bool display_pool_message()
  {
    return liquid_volumes.pool_volume > 2.f && pool_msg_displayed.once();
  }
  
  bool is_exercise_completed() const
  {
    return cystic_artery_divided() && cystic_duct_divided()
      && (!has_flowed_water || has_drained_water) && has_drained_blood && has_drained_bile;
  }
  
  bool leaky_arteria_hepatica_transient() const { return leaky_arteria_hepatica.pos_flank(); }
  bool leaky_ductus_hepaticus_transient() const { return leaky_ductus_hepaticus.pos_flank(); }
  bool leaky_arteria_gastroduodenalis_transient() const { return leaky_arteria_gastroduodenalis.pos_flank(); }
  bool leaky_ductus_choledochus_transient() const { return leaky_ductus_choledochus.pos_flank(); }
  bool duct_root_damaged_transient() const { return damaged_duct_root.pos_flank(); }
  
};

///

template<int NR, int NC>
void handle_injuries(ScreenHandler<NR, NC>& sh, t8x::ui::MessageHandler& mh,
                     HealthStates& health_states,
                     int& health, int& blood, const int max_blood, int& score,
                     float time)
{
  using MessageHandler = t8x::ui::MessageHandler;

  if (!mh.has_message(time))
  {
    // Most severe states last.
    if (health_states.display_pool_message()) // #FIXME: Fix repeat-message bug.
    {
      mh.add_message(time, "Please use the Rinsing and Suction Device to drain the fluids.", MessageHandler::Level::Guide);
    }
    if (health_states.artery_tree_leaking_trans())
    {
      mh.add_message(time, "Stop the bleeding!", MessageHandler::Level::Warning);
      score -= 50;
    }
    if (health_states.duct_tree_leaking_trans())
    {
      mh.add_message(time, "The Bile Duct is leaking!", MessageHandler::Level::Warning);
      score -= 50;
    }
    if (health_states.cut_transient_gallbladder())
    {
      mh.add_message(time, "You cut on the Gallbladder!", MessageHandler::Level::Warning);
      health--;
      score -= 100;
    }
    if (health_states.clipped_transient_gallbladder())
    {
      mh.add_message(time, "You clipped the Gallbladder!", MessageHandler::Level::Warning);
      health--;
      score -= 100;
    }
    if (health_states.bleeding_critical_trans())
    {
      mh.add_message(time, "Bleeding is critical!", MessageHandler::Level::Fatal);
      score -= 300;
    }
    if (health_states.burned_transient_liver())
    {
      mh.add_message(time, "You burned on the Liver!", MessageHandler::Level::Fatal);
      health -= 6;
      score -= 300;
    }
    if (health_states.cut_transient_liver())
    {
      mh.add_message(time, "You cut on the Liver!", MessageHandler::Level::Fatal);
      health -= 6;
      score -= 300;
    }
    if (health_states.clipped_transient_liver())
    {
      mh.add_message(time, "You clipped the Liver!", MessageHandler::Level::Fatal);
      health -= 3;
      score -= 300;
    }
    if (health_states.duct_root_damaged_transient())
    {
      mh.add_message(time, "Duct Root damaged! Unrecoverable injury!", MessageHandler::Level::Fatal);
      health = 0;
      score -= 1000;
    }
    if (health_states.leaky_arteria_hepatica_transient())
    {
      mh.add_message(time, "You damaged Arteria Hepatica!", MessageHandler::Level::Fatal);
      health = 0;
      score -= 1000;
    }
    if (health_states.leaky_ductus_hepaticus_transient())
    {
      mh.add_message(time, "You damaged Ductus Hepaticus!", MessageHandler::Level::Fatal);
      health = 0;
      score -= 1000;
    }
    if (health_states.leaky_arteria_gastroduodenalis_transient())
    {
      mh.add_message(time, "You damaged Arteria Gastroduodenalis!", MessageHandler::Level::Fatal);
      health = 0;
      score -= 1000;
    }
    if (health_states.leaky_ductus_choledochus_transient())
    {
      mh.add_message(time, "You damaged Ductus Choledochus!", MessageHandler::Level::Fatal);
      health = 0;
      score -= 1000;
    }
    if (health_states.burned_transient_gallbladder())
    {
      mh.add_message(time, "You burned on the Gallbladder!", MessageHandler::Level::Warning);
      health--;
      score -= 100;
    }
  }
  
  float blood_val = math::linmap(health_states.get_blood_vol(),
                           0.f, health_states.get_crit_blood_vol(),
                           0.f, static_cast<float>(max_blood));
  blood = math::roundI(blood_val);
  
  mh.update(sh, time);
}
