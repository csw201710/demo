int
testOpt(int argc, char **argv)
{
	int c;
	int digit_optind = 0;
	int option_index = 0;
	char *optstring = "a:b:c:d";
	int lopt = 0;
	static struct option long_options[] = {
		{ "reqarg", required_argument, NULL, 'r' },
		{ "noarg",  no_argument,       NULL, 'n' },
		{ "optarg", optional_argument, NULL, 'o' },
		{ "http", required_argument, &lopt, 3 },
		{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
	{
		printf("getopt_long ret = %c\n", c);
		if (c == 0) {
			printf("lopt  = %d\n", lopt); //3
		}
		printf("optarg = %s\n", optarg);
		printf("optind = %d\n", optind);
		printf("argv[optind - 1] = %s\n", argv[optind - 1]);
		printf("option_index = %d\n", option_index); // long_options 数组的当前索引
		printf("\n");
	}

	return 0;
}

