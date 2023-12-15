/*
	Generate throughtput matrix file based on the graph adjacency matrix file.

	Params:
	input_file_path		- adjacency matrix file path
	output_file_path	- output throughtput matrix file path
	nodes_amount		- number of vertices

	Return:
	None
*/
void Data::generate_throughtput(std::string input_file_path, std::string output_file_path, uint32_t nodes_amount)
{
	std::ofstream o_file(output_file_path);
	std::ifstream i_file(input_file_path);

	srand(time(NULL));

	uint32_t value;

	if (i_file.good())
	{
		// generate a matrix of throughtput for given graph file
		for (uint32_t i = 0; i < nodes_amount; i++)
		{
			for (uint32_t j = 0; j < nodes_amount; j++)
			{
				i_file >> value;
				if (value != 0)
				{
					o_file << rand() % 9 + 1;
				}
				else
				{
					o_file << 0;
				}
				if (j < nodes_amount - 1)
				{
					o_file << " ";
				}
			}
			if (i < nodes_amount - 1)
			{
				o_file << std::endl;
			}
		}
	}
	o_file.close();
	i_file.close();
}
