/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:01:17 by vlnikola          #+#    #+#             */
/*   Updated: 2026/04/13 15:24:09 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/**
 * Supported dongle arbitration policies.
 */
typedef enum e_scheduler {
  CODEX_SCHED_FIFO = 0,
  CODEX_SCHED_EDF = 1
} t_scheduler;

/**
 * Runtime configuration parsed from CLI arguments.
 */
typedef struct s_config {
  int number_of_coders;
  int time_to_burnout;
  int time_to_compile;
  int time_to_debug;
  int time_to_refactor;
  int number_of_compiles_required;
  int dongle_cooldown;
  t_scheduler scheduler;
} t_config;

/* Forward declarations */
typedef struct s_sim t_sim;
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;

/**
 * Represents a single queue request for a dongle.
 */
typedef struct s_req {
  t_coder *coder;
  long long arrival_time;
  long long deadline;
  struct s_req *next;
} t_req;

/**
 * Represents a shared USB dongle resource.
 */
struct s_dongle {
  int id;
  pthread_mutex_t lock;
  pthread_cond_t cond;
  int is_available;
  long long available_at; // Timestamp when cooldown ends
  t_req *queue_head;      // Arbitration queue for requests
};

/**
 * Represents a Coder thread.
 */
struct s_coder {
  int id;
  pthread_t thread;
  t_dongle *left;
  t_dongle *right;
  long long last_compile_start;
  int compile_count;
  t_sim *sim;
};

/**
 * Represents the overall Simulation state.
 */
struct s_sim {
  t_config config;
  t_coder *coders;
  t_dongle *dongles;
  pthread_mutex_t print_lock;
  pthread_mutex_t monitor_lock;
  int stop_flag;
};

#endif