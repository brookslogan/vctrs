#ifndef VCTRS_ARG_COUNTER_H
#define VCTRS_ARG_COUNTER_H

#include "vctrs-core.h"

struct counters {
 /* public: */

  r_obj* shelter;

  // Argument tags for the current value of the reduction (the result
  // so far) and the next value. These handles typically point to the
  // local counter args, but might also point to external arg objects
  // like the initial current arg, or a splice box counter arg.
  struct vctrs_arg* curr_arg;
  struct vctrs_arg* next_arg;

 /* private: */

  // Global counters
  r_ssize curr;
  r_ssize next;

  r_obj* names;
  r_ssize names_curr;
  r_ssize names_next;

  // `names` might be from a splice box whose reduction has already
  // finished. We protect those from up high.
  r_keep_loc names_pi;

  // Local counters for splice boxes. Since the tags are generated
  // lazily, we need two counter states to handle the
  // `vec_c(!!!list(foo = 1), !!!list(bar = 2))` case.
  struct counters* next_box_counters;
  struct counters* prev_box_counters;

  struct vctrs_arg curr_counter;
  struct vctrs_arg next_counter;

  void* p_data;
};

/**
 * Swap the argument tags of the reduction
 *
 * There are two counters used for generating argument tags when an
 * error occur during a reduction. The first represent the result so
 * far, and the second the next input. Call `counters_shift()` to set
 * the counter of the next input as current counter, and start
 * iterating with a new counter for the next input.
 */
void counters_shift(struct counters* counters);

r_obj* reduce(r_obj* current,
              struct vctrs_arg* p_current_arg,
              struct vctrs_arg* p_parent_arg,
              r_obj* rest,
              r_obj* (*impl)(r_obj* current, r_obj* next, struct counters* counters, void* data),
              void* data);


#endif
