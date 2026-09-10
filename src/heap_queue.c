/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:01 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:39:13 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = malloc(sizeof(t_request) * capacity);
	if (!heap->nodes)
	{
		heap->size = -1;
		heap->capacity = 0;
		return ;
	}
	heap->size = 0;
	heap->capacity = capacity;
}

int	is_higher_priority(t_request a, t_request b, int scheduler)
{
	if (!scheduler)
	{
		if (a.enter_time != b.enter_time)
			return (a.enter_time < b.enter_time);
		return (a.id < b.id);
	}
	if (a.time_to_burnout != b.time_to_burnout)
		return (a.time_to_burnout < b.time_to_burnout);
	if (a.enter_time != b.enter_time)
		return (a.enter_time < b.enter_time);
	return (a.id < b.id);
}

void	heap_push(t_heap *heap, t_request *node, int scheduler)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->nodes[heap->size] = *node;
	heap->size++;
	heapify_up(heap, heap->size - 1, scheduler);
}

t_request	heap_pop(t_heap *heap, int scheduler)
{
	t_request	res;

	heap->size--;
	res = heap->nodes[0];
	heap->nodes[0] = heap->nodes[heap->size];
	heapify_down(heap, 0, scheduler);
	return (res);
}
