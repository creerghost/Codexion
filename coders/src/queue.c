#include "codexion.h"

void	fifo_insert(t_dongle *dongle, t_req *req)
{
	t_req *curr;

	req->next = NULL;
	if (dongle->queue_head == NULL)
		dongle->queue_head = req;
	else
	{
		curr = dongle->queue_head;
		while (curr->next != NULL)
			curr = curr->next;
		curr->next = req;
	}
}

void	edf_insert(t_dongle *dongle, t_req *req)
{
	t_req *curr;
	t_req *prev;

	req->next = NULL;
	if (dongle->queue_head == NULL)
		dongle->queue_head = req;
	else
	{
		curr = dongle->queue_head;
		prev = NULL;
		while (curr != NULL && curr->deadline <= req->deadline)
		{
			prev = curr;
			curr = curr->next;
		}
		if (prev == NULL)
			dongle->queue_head = req;
		else
			prev->next = req;
		req->next = curr;
	}
}

void	enqueue_req(t_dongle *dongle, t_req *req, t_sim *sim)
{
	if (sim->config.scheduler == CODEX_SCHED_FIFO)
		fifo_insert(dongle, req);
	else
		edf_insert(dongle, req);
}

t_req	*dequeue_req(t_dongle *dongle)
{
	t_req *req;

	if (dongle->queue_head == NULL)
		return (NULL);
	req = dongle->queue_head;
	dongle->queue_head = dongle->queue_head->next;
	return (req);
}

int	is_turn(t_dongle *dongle, t_req *req)
{
	return (dongle->queue_head == req);
}
