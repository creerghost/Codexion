/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 09:20:04 by vlnikola          #+#    #+#             */
/*   Updated: 2026/05/08 09:21:19 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
#define QUEUE_H

#include "codexion.h"

void fifo_insert(t_dongle *dongle, t_req *req);
void edf_insert(t_dongle *dongle, t_req *req);
void enqueue_req(t_dongle *dongle, t_req *req, t_sim *sim);
t_req *dequeue_req(t_dongle *dongle);
int is_turn(t_dongle *dongle, t_req *req);

#endif