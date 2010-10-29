/*
 *  primitive.cc
 *  Avida
 *
 *  Copyright 1999-2010 Michigan State University. All rights reserved.
 *
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; version 2
 *  of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "AvidaTools.h"

#include "avida.h"
#include "cAvidaConfig.h"
#include "cDefaultAnalyzeDriver.h"
#include "cDefaultRunDriver.h"
#include "cWorld.h"
#include "defs.h"

using namespace std;


int main(int argc, char * argv[])
{
  Avida::Initialize();
  
  Avida::PrintVersionBanner();

  // Initialize the configuration data...
  cAvidaConfig* cfg = new cAvidaConfig();
  Avida::ProcessCmdLineArgs(argc, argv, cfg);
  
  tList<cString> errors;
  cWorld* world = cWorld::Initialize(cfg, AvidaTools::FileSystem::GetCWD(), &errors);

  if (!world) {
    tListIterator<cString> it(errors);
    while ((it.Next())) {
      cerr << "error: " << *it.Get() << endl;
      delete it.Get();
    }
    return -1;
  }
  
  const int rand_seed = world->GetConfig().RANDOM_SEED.Get();
  cout << "Random Seed: " << rand_seed;
  if (rand_seed != world->GetRandom().GetSeed()) cout << " -> " << world->GetRandom().GetSeed();
  cout << endl;

  if (world->GetConfig().VERBOSITY.Get() > VERBOSE_NORMAL)
    cout << "Data Directory: " << world->GetDataFileManager().GetTargetDir() << endl;

  cAvidaDriver* driver = NULL;

  if (world->GetConfig().ANALYZE_MODE.Get() > 0) {
    driver = new cDefaultAnalyzeDriver(world, (world->GetConfig().ANALYZE_MODE.Get() == 2));
  } else {
    driver = new cDefaultRunDriver(world);
  }

  cout << endl;
  
  driver->Run();

  // Exit Nicely
  Avida::Exit(0);
  
  return 0;
}
