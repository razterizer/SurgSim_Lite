#pragma once

namespace liquids
{
struct LiquidVolumes
  {
    float pool_volume = 0.f;
    float blood_volume = 0.f;
    float bile_volume = 0.f;
    float water_volume = 0.f;
  
    float t_blood() const { return pool_volume == 0.f ? 0.f : blood_volume / pool_volume; }
    float t_bile() const { return pool_volume == 0.f ? 0.f : bile_volume / pool_volume; }
    float t_water() const { return pool_volume == 0.f ? 0.f : water_volume / pool_volume; }
    float t_blood_bile() const
    {
      auto tot_vol = blood_volume + bile_volume;
      return tot_vol == 0.f ? 0.f : bile_volume / tot_vol;
    }
    float t_bile_water() const
    {
      auto tot_vol = bile_volume + water_volume;
      return tot_vol == 0.f ? 0.f : water_volume / tot_vol;
    }
    float t_water_blood() const
    {
      auto tot_vol = water_volume + blood_volume;
      return tot_vol == 0.f ? 0.f : blood_volume / tot_vol;
    }
  
    void operator-= (float vol)
    {
      pool_volume -= vol;
      blood_volume -= vol * t_blood();
      bile_volume -= vol * t_bile();
      water_volume -= vol * t_water();
    }
  
    void add(LiquidType type, float vol)
    {
      pool_volume += vol;
      switch(type)
      {
        case LiquidType::Blood: blood_volume += vol; break;
        case LiquidType::Bile: bile_volume += vol; break;
        case LiquidType::Water: water_volume += vol; break;
        case LiquidType::NUM_ITEMS: break;
      }
    }
  
    void update()
    {
      if (pool_volume < 0.f)
        pool_volume = 0.f;
      if (blood_volume < 0.f)
        blood_volume = 0.f;
      if (bile_volume < 0.f)
        bile_volume = 0.f;
      if (water_volume < 0.f)
        water_volume = 0.f;
    }
  };
}
