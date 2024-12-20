#pragma once

enum class Fault {
  NO_FAULT,
  NOT_IMPLEMENTED,
  ILLEGAL_INSTRUCTION,
  LOAD_ACCESS_FAULT,
  STORE_ACCESS_FAULT,
  THE_END_OF_TASK
};

enum class PageFault {
  NoFault,
  InvalidPage,
  WButNotR,
  ILessThenZero,
  AccessType,
  AccessBit,
  DirtyBit,
  SuperPage
};