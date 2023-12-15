/*
	Generates a random weighted graph in given destination.

	Params:
	file_path		- string containing the path for the output file
	nodes_amount	- size of the graph

	Return:
	None
*/
void Data::generate_weighted_graph(std::string file_path, uint32_t nodes_amount)
{
	std::ofstream file(file_path);

	srand(time(NULL));

	if (file.good())
	{
		// generate a graph of given size with random weights in the 
		// range of <0;n>. Zero means no connection between vertices.
		for (uint32_t i = 0; i < nodes_amount; i++)
		{
			for (uint32_t j = 0; j < nodes_amount; j++)
			{
				if (i != j)
				{
					file << (rand() % nodes_amount) * (rand() % 6 <= 3 ? 1 : 0);
				}
				else
				{
					file << 0;
				}
				if (j < nodes_amount - 1)
				{
					file << " ";
				}
			}
			if (i < nodes_amount - 1)
			{
				file << std::endl;
			}
		}
	}
	file.close();
}