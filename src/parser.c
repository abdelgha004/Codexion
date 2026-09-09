
#include "../codexion.h"

static void	ft_perror_part2(int i)
{
	if (i == 9)
		fprintf(stderr, "Error: time to debug should be greater than 0.\n");
	else if (i == 10)
		fprintf(stderr, "Error: time to refactor should be greater than 0.\n");
	else if (i == 11)
		fprintf(stderr,
			"Error: number of compiles required should be greater than 0.\n");
	else if (i == 12)
		fprintf(stderr, "Error: dongle cooldown should not be negative.\n");
	else if (i == 14)
		fprintf(stderr, "Error: Couldnt create all threads.\n");
	else if (i == 18)
		fprintf(stderr, "Error: Max number of coders exceeded (3000).\n");
	else if (i == 21)
		fprintf(stderr, "Error: Initializing global mutexes failed.\n");
	else if (i == 22)
		fprintf(stderr, "Error: Initializing coders or dongles failed.\n");
	else if (i == 16)
		fprintf(stderr, "Allocation Error!\n");
}

void	ft_perror(int i)
{
	if (i == 0)
		return ;
	if (i == 1)
		fprintf(stderr, "Error: expected exactly 8 arguments.\n");
	else if (i == 2)
		fprintf(stderr, "Error: argument must not be empty.\n");
	else if (i == 3)
		fprintf(stderr, "Error: integer overflow.\n");
	else if (i == 4)
		fprintf(stderr, "Error: invalid argument.\n");
	else if (i == 5)
		fprintf(stderr, "Error: Scheduler must be 'edf' or 'fifo'.\n");
	else if (i == 6)
		fprintf(stderr, "Error: number of coders should be greater than 0.\n");
	else if (i == 7)
		fprintf(stderr, "Error: time to burnout should be greater than 0.\n");
	else if (i == 8)
		fprintf(stderr, "Error: time to compile should be greater than 0.\n");
	else
		ft_perror_part2(i);
}

static void	fill_targets(int **targets, t_config *conf)
{
	targets[0] = &conf->number_of_coders;
	targets[1] = &conf->time_to_burnout;
	targets[2] = &conf->time_to_compile;
	targets[3] = &conf->time_to_debug;
	targets[4] = &conf->time_to_refactor;
	targets[5] = &conf->number_of_compiles_required;
	targets[6] = &conf->dongle_cooldown;
}

int	parser(int argc, char **argv, t_config *conf)
{
	int	i;
	int	*targets[7];
	int	v;

	i = 0;
	if (argc != 9)
		return (1);
	fill_targets(targets, conf);
	while (++i < 8)
	{
		v = ft_atoi(argv[i], targets[i - 1]);
		if (v)
			return (v);
	}
	if (!strcmp(argv[i], "fifo"))
		conf->scheduler = 0;
	else if (!strcmp(argv[i], "edf"))
		conf->scheduler = 1;
	else
		return (5);
	return (0);
}
