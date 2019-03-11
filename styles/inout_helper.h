#ifndef STYLES_INOUT_HELPER_H
#define STYLES_INOUT_HELPER_H

// InOutHelperX<BASE, EXTENSION, OFF_COLOR>
// BASE, OFF_COLOR: COLOR
// EXTENSION: FUNCTION
// return value: COLOR
// This class does a basic extend/retract. Basically it fades between
// BASE and OFF_COLOR (which defaults to black). The amount of extension
// is determined by EXTENSION. If EXTENSION returns 32768, the blade is fully
// extended. If it returns zero, it is not extended.

template<class T, class EXTENSION, class OFF_COLOR=Rgb<0,0,0>, bool ALLOW_DISABLE=1 >
class InOutHelperX {
public:
  bool run(BladeBase* blade) __attribute__((warn_unused_result)) {
    base_.run(blade);
    off_color_.run(blade);
    extension_.run(blade);
    thres = (extension_.getInteger(0) * blade->num_leds()) >> 7;

    if (ALLOW_DISABLE && is_same_type<OFF_COLOR, Rgb<0,0,0> >::value && thres == 0)
      return false;
    return true;
  }
  OverDriveColor getColor(int led) {
    int black_mix = clampi32(thres - led * 256, 0, 255);
    OverDriveColor ret = base_.getColor(led);
    OverDriveColor off_color  = off_color_.getColor(led);
    ret.c = off_color.c.mix(ret.c, black_mix);
    return ret;
  }
private:
  T base_;
  OFF_COLOR off_color_;
  EXTENSION extension_;
  int thres = 0;
};

// Usage: InOutHelper<BASE, OUT_MILLIS, IN_MILLIS>
// or: InOutHelper<BASE, OUT_MILLIS, IN_MILLIS, OFF_COLOR>
// BASE, OFF_COLOR: COLOR
// OUT_MILLIS, IN_MILLIS: a number
// return value: COLOR
// This class does a basic extend/retract. Basically it fades between
// BASE and OFF_COLOR (which defaults to black). It starts by just
// displaying OFF_COLOR, and when you turn the saber on it starts mixing
// in BASE at the base of the saber. After OUT_MILLIS milliseconds, it
// will be displaying the BASE color on the entire blade.

#include "../functions/ifon.h"
  
template<class T, int OUT_MILLIS, int IN_MILLIS, class OFF_COLOR=Rgb<0,0,0> >
  using InOutHelper = InOutHelperX<T, InOutFunc<OUT_MILLIS, IN_MILLIS>, OFF_COLOR>;

#endif
