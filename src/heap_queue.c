
#include "../codexion.h"

void	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = malloc(sizeof(t_request) * capacity);
	if (!heap->nodes)
	{
		heap->size = 0;
		heap->capacity = 0;
		return ;
	}
	heap->size = 0;
	heap->capacity = capacity;
}

void	heap_free(t_heap *heap)
{
	free(heap->nodes);
	heap->nodes = NULL;
	heap->size = 0;
	heap->capacity = 0;
}

int	is_higher_priority(t_request a, t_request b, int scheduler)
{
	if (!scheduler)
	{
		if (a.enter_time == b.enter_time)
			return (a.id < b.id);
		return (a.enter_time < b.enter_time);
	}
	if (a.time_to_burnout == b.time_to_burnout)
	{
		if (a.enter_time == b.enter_time)
			return (a.id < b.id);
		return (a.enter_time < b.enter_time);
	}
	return (a.time_to_burnout < b.time_to_burnout);
}

int	heap_push(t_heap *heap, t_request *node, int scheduler)
{
	if (heap->size >= heap->capacity)
		return (1);
	heap->nodes[heap->size] = *node;
	heap->size++;
	heapify_up(heap, heap->size - 1, scheduler);
	return (0);
}

t_request	heap_pop(t_heap *heap, int scheduler)
{
	t_request	res;

	res = heap->nodes[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->nodes[0] = heap->nodes[heap->size];
		heapify_down(heap, 0, scheduler);
	}
	return (res);
}
