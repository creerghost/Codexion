/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:07:49 by vlnikola          #+#    #+#             */
/*   Updated: 2026/04/15 18:04:29 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "parse.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/**
 * Parse one numeric argument and store it in the destination field.
 * Returns 1 on success, 0 on invalid input.
 */
static int	parse_int_arg(const char *arg, int *dst, const char *label)
{
	long	value;

	if (!uatol(arg, &value))
	{
		fprintf(stderr, "ERROR: invalid %s (must be a non-negative integer)\n",
			label);
		return (0);
	}
	*dst = (int)value;
	return (1);
}

/**
 * Parse scheduler argument as either fifo or edf.
 * Returns 1 on success, 0 on invalid scheduler value.
 */
static int	parse_scheduler(const char *arg, t_scheduler *scheduler)
{
	if (strcmp(arg, "fifo") == 0)
		*scheduler = CODEX_SCHED_FIFO;
	else if (strcmp(arg, "edf") == 0)
		*scheduler = CODEX_SCHED_EDF;
	else
	{
		fprintf(stderr,
			"ERROR: invalid scheduler (must be exactly 'fifo' or 'edf')\n");
		return (0);
	}
	return (1);
}

/**
 * Initialize mapping between numeric CLI arguments and config fields.
 */
static void	init_int_args(t_int_arg *args, t_config *config)
{
	args[0] = (t_int_arg){&config->number_of_coders, "number_of_coders"};
	args[1] = (t_int_arg){&config->time_to_burnout, "time_to_burnout"};
	args[2] = (t_int_arg){&config->time_to_compile, "time_to_compile"};
	args[3] = (t_int_arg){&config->time_to_debug, "time_to_debug"};
	args[4] = (t_int_arg){&config->time_to_refactor, "time_to_refactor"};
	args[5] = (t_int_arg){&config->number_of_compiles_required,
		"number_of_compiles_required"};
	args[6] = (t_int_arg){&config->dongle_cooldown, "dongle_cooldown"};
}

/**
 * Parse the seven numeric arguments (argv[1]..argv[7]).
 * Returns 1 if all are valid, otherwise 0.
 */
static int	parse_numeric_args(char **argv, t_int_arg *args)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!parse_int_arg(argv[i + 1], args[i].field, args[i].label))
			return (0);
		i++;
	}
	return (1);
}

/**
 * Parse and validate all program arguments into the config structure.
 * Expected format: ./codexion <7 numeric args> <fifo|edf>.
 */
int	parse_args(int argc, char **argv, t_config *config)
{
	t_int_arg	args[7];

	if (argc != 9)
	{
		fprintf(stderr,
			"ERROR: usage: ./codexion number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown scheduler\n");
		return (0);
	}
	init_int_args(args, config);
	if (!parse_numeric_args(argv, args))
		return (0);
	if (!parse_scheduler(argv[8], &config->scheduler))
		return (0);
	if (config->number_of_coders < 1)
	{
		fprintf(stderr, "ERROR: number_of_coders must be at least 1\n");
		return (0);
	}
	return (1);
}
