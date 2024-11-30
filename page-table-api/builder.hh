
// Cannot use any libs
#pragma once
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef enum PTE_type {
  NextLevel = 0b000,
  RO = 0b001,
  RW = 0b011,
  XO = 0b100,
  RX = 0b101,
  RWX = 0b111
} PTE_type;

template <typename type> class PageTableBuilderBase {
public:
  uint32_t page_offset = 12;
  using Addr = type;
  using Reg = type;

  virtual void SetRootPPN(Addr ppn) = 0;
  virtual void FirstLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type) = 0;
  virtual void SecondLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type) = 0;
  virtual void EnableTranslation() = 0;
  virtual void DisableTranslation() = 0;

  virtual inline Addr NormalizeVPN(Addr vpn) = 0;
  virtual inline Addr NormalizePPN(Addr ppn) = 0;
};

class Sv32PageTableBuilder : public PageTableBuilderBase<uint32_t> {
  void SetRootPPN(Addr ppn) override;
  void FirstLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type) override;
  void SecondLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type) override;
  void EnableTranslation() override;
  void DisableTranslation() override;

  virtual inline Addr NormalizeVPN(Addr vpn) override;
  virtual inline Addr NormalizePPN(Addr ppn) override;
};

class PageTableBuilder64bit : public PageTableBuilderBase<uint64_t> {
public:
  Addr saptMaskPPN = (1ULL << 43) - 1;
  // void SetRootPPN(Addr ppn) override;
  // void DisableTranslation() override;

  void SetRootPPN(Addr ppn) override {
    ppn = NormalizePPN(ppn);
    Reg satp;
    asm("csrrs %[dest], satp, x0" : [dest] "=r"(satp)); // read satp
    satp |= ppn;
    asm("csrrw x0, satp, %[value]" ::[value] "r"(satp)); // write satp
  }

  void DisableTranslation() override {
    Reg satp;
    asm("csrrs %[dest], satp, x0" : [dest] "=r"(satp)); // read satp
    satp &= ((1ULL << 60) - 1);
    asm("csrrw x0, satp, %[value]" ::[value] "r"(satp)); // write satp
  }
};

class Sv39PageTableBuilder : public PageTableBuilder64bit {
public:
  void FirstLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type) override;
  void SecondLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type) override;
  void ThirdLevelPtCnfg(Addr vpn, Addr ppn, PTE_type pte_type);
  void EnableTranslation() override;

private:
  virtual inline Addr NormalizeVPN(Addr vpn) override;
  virtual inline Addr NormalizePPN(Addr ppn) override;
};

class Sv48PageTableBuilder : public PageTableBuilder64bit {};