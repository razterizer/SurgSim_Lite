#pragma once

enum class Key
{
  None,
  LI_Left,
  LI_Right,
  LI_Down,
  LI_Up,
  RI_Left,
  RI_Right,
  RI_Down,
  RI_Up,
  Coag,
  Cut,
  LI_Trigger,
  LI_Trigger2,
  RI_Trigger,
  RI_Trigger2,
  Menu,
  Skip,
  Pause,
  NUM_ITEMS
};
enum class InstrumentSide { Left, Right, NUM_ITEMS };
enum class ToolType { Grasper, Hook, Scissors, ClipApplier, SuctionRinsing, NUM_ITEMS };
enum class TextureType { HD_LIG, ARTERY_TREE, DUCT_TREE, GALLBLADDER, LIVER, NUM_ITEMS };
enum class LiquidType { Blood, Bile, Water, NUM_ITEMS };
