/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:45:00 by vlnikola          #+#    #+#             */
/*   Updated: 2026/04/15 15:45:00 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <sys/time.h>
#include <unistd.h>

/**
 * Returns the current time in milliseconds.
 */
long long get_time_ms(void) {
  struct timeval tv;

  if (gettimeofday(&tv, NULL) == -1)
    return (-1);
  return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

/**
 * Sleeps for a precise amount of milliseconds.
 * Uses a while loop with small usleep calls to prevent early wake-ups.
 */
void precise_sleep(long long time_in_ms) {
  long long start;

  start = get_time_ms();
  while ((get_time_ms() - start) < time_in_ms)
    usleep(200);
}
