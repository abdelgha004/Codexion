
#include "../codexion.h"
#include <pthread.h>

int	main(int argc, char **argv)
{
	t_config	conf;
	int			valid;

	valid = 0;
	memset(&conf, 0, sizeof(t_config));
	valid = full_checker(argc, argv, &conf);
	if (valid)
		return (ft_perror(valid), valid);
	valid = initialize_data(&conf);
	if (!valid)
		valid = run_simulation(&conf);
	return (ft_perror(valid), clean_data(&conf), valid);
}
