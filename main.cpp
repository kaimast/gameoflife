#include "Game.h"
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()
          ("tile-size", po::value<uint32_t>(), "Set the size of a tile")
          ("no-graphics", "Disable Graphics")
          ("initial-seed", po::value<uint32_t>(), "Set the size of the initial seed")
          ("benchmark",   "Use benchmark mode");

    po::variables_map vm;

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch(po::error& e)
    {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      return -1;
    }

    bool graphics = vm.count("no-graphics") == 0;
    bool benchmark = vm.count("benchmark") > 0;

    uint32_t tileSize = 100;

    if(vm.count("tile-size") > 0) {
        tileSize = vm["tile-size"].as<uint32_t>();
    }

    uint32_t initialSeed = 1000;

    if(vm.count("initial-seed") > 0) {
        initialSeed = vm["initial-seed"].as<uint32_t>();
    }

    Game game(benchmark, tileSize, initialSeed);

    if(!game.init(graphics))
        return -1;

    while(game.isOk())
        game.update();

    return 0;
}

