#include <Config.h>
#include <Serial.h>

using namespace std;

std::vector<script_t> ScriptStack;

void Config_Init()
{
  ////printf("Config!\n");

  //
  ifstream ifs;
  string line;
  vector<string> array;
  object_t tmpObject;
  ifs.open("GaugePanel.conf");
  if (ifs.is_open())
  {
    ObjectStack.clear();
    while (getline (ifs, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end()); //Remove all newline characters
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); //Remove all carage return characters
        //printf("%sLine%s = %s\n", KRED, KBLUE, line.c_str());
        if (line.size() > 0)
        {
          if (line.at(0) != '#')
          {
            array = split(line, ' ');
            for (int x = 0; x < array.size(); x++)
            {
              //printf("%sArray[%d]%s = %s\n", KRED, x, KBLUE, array[x].c_str());
            }
            if (array[0] == "WindowSize")
            {
              if (array.size() == 3)
              {
                //printf("%sSetting Window Size!\n%s", KGREEN, KNORMAL);
                SCREEN_WIDTH = atoi(array[1].c_str());
                SCREEN_HEIGHT = atoi(array[2].c_str());
              }
            }
            if (array[0] == "BackgroundColor")
            {
              if (array.size() == 4)
              {
                //printf("%sSetting Background Color!\n%s", KGREEN, KNORMAL);
                BackgroundColor = color_t{ (Uint8)atoi(array[1].c_str()), (Uint8)atoi(array[2].c_str()), (Uint8)atoi(array[3].c_str()) };
              }
            }
            if (array[0] == "BackgroundImage")
            {
              if (array.size() == 6)
              {
                //printf("%sSetting Background Image!\n%s", KGREEN, KNORMAL);
                tmpObject.zindex = -1;
                tmpObject.tagname = "BackgroundImage";
                tmpObject.position.x = atoi(array[1].c_str());
            		tmpObject.position.y = atoi(array[2].c_str());
            		tmpObject.size.w = atoi(array[3].c_str());
            		tmpObject.size.h = atoi(array[4].c_str());
                tmpObject.texture = Render_loadImage( array[5] );
                tmpObject.angle = 0;
                tmpObject.visable = true;
                if( tmpObject.texture == NULL )
                {
                  //printf("%s\tCan't open %s%s%s\n", KRED, KBLUE, array[5].c_str() ,KNORMAL);
                }
                else
                {
                  ObjectStack.push_back(tmpObject);
                }
              }
            }
            if (array[0] == "NeedleImage")
            {
              if (array.size() == 11)
              {
                //#NeedleImage TagName Xoffset Yoffset Xscale Yscale MinAngle MaxAngle MinValue MaxValue /path/to/image
                //printf("%sSetting Needle Image!\n%s", KGREEN, KNORMAL);
                tmpObject.type = NEEDLE;
                tmpObject.zindex = 2;
                tmpObject.tagname = array[1];
                tmpObject.position.x = atoi(array[2].c_str());
            		tmpObject.position.y = atoi(array[3].c_str());
            		tmpObject.size.w = atoi(array[4].c_str());
            		tmpObject.size.h = atoi(array[5].c_str());
                tmpObject.minangle = atoi(array[6].c_str());
                tmpObject.maxangle = atoi(array[7].c_str());
                tmpObject.minvalue = atoi(array[8].c_str());
                tmpObject.maxvalue = atoi(array[9].c_str());

                tmpObject.angle = 0;
                tmpObject.visable = true;

                tmpObject.texture = Render_loadImage( array[10] );
                if( tmpObject.texture == NULL )
                {
                  //printf("%s\tCan't open %s%s%s\n", KRED, KBLUE, array[10].c_str() ,KNORMAL);
                }
                else
                {
                  ObjectStack.push_back(tmpObject);
                }
              }
            }
            if (array[0] == "SerialDevice")
            {
              if (array.size() == 2)
              {
                //printf("%sSetting Serial Device!\n%s", KGREEN, KNORMAL);
                SerialDevice = array[1];
              }
            }
            if (array[0] == "IndicatorImage")
            {
              //IndicatorImage TagName Xoffset Yoffset Xscale Yscale Angle Visability /path/to/image
              if (array.size() == 9)
              {
                //printf("%sAdding Indicator!\n%s", KGREEN, KNORMAL);
                tmpObject.type = INDICATOR;
                tmpObject.tagname = array[1];
                tmpObject.zindex = 1;
                tmpObject.position.x = atoi(array[2].c_str());
                tmpObject.position.y = atoi(array[3].c_str());
                tmpObject.size.w = atoi(array[4].c_str());
                tmpObject.size.h = atoi(array[5].c_str());
                tmpObject.angle = atoi(array[6].c_str());
                if (array[7] == "True")
                {
                  tmpObject.visable = true;
                }
                else
                {
                  tmpObject.visable = false;
                }
                tmpObject.texture = Render_loadImage( array[8] );
                if( tmpObject.texture == NULL )
                {
                  //printf("%s\tCan't open %s%s%s\n", KRED, KBLUE, array[8].c_str() ,KNORMAL);
                }
                else
                {
                  ObjectStack.push_back(tmpObject);
                }
              }
            }
            if (array[0] == "GaugeImage")
            {
              //GaugeImage TagName Xoffset Yoffset Xscale Yscale Angle /path/to/image
              if (array.size() == 8)
              {
                //printf("%sAdding Gauge!\n%s", KGREEN, KNORMAL);
                tmpObject.visable = true;
                tmpObject.type = GAUGE;
                tmpObject.zindex = 1;
                tmpObject.tagname = array[1];
                tmpObject.position.x = atoi(array[2].c_str());
                tmpObject.position.y = atoi(array[3].c_str());
                tmpObject.size.w = atoi(array[4].c_str());
                tmpObject.size.h = atoi(array[5].c_str());
                tmpObject.angle = atoi(array[6].c_str());
                tmpObject.texture = Render_loadImage( array[7] );
                if( tmpObject.texture == NULL )
                {
                  //printf("%s\tCan't open %s%s%s\n", KRED, KBLUE, array[7].c_str() ,KNORMAL);
                }
                else
                {
                  ObjectStack.push_back(tmpObject);
                }
              }
            }
            if (array[0] == "ButtonImage")
            {
              //ButtonImage TagName Xoffset Yoffset Xscale Yscale Angle Visability /path/to/image
              if (array.size() == 9)
              {
                //printf("%sAdding Gauge!\n%s", KGREEN, KNORMAL);
                tmpObject.visable = true;
                tmpObject.type = INPUT;
                tmpObject.zindex = 1;
                tmpObject.tagname = array[1];
                tmpObject.position.x = atoi(array[2].c_str());
                tmpObject.position.y = atoi(array[3].c_str());
                tmpObject.size.w = atoi(array[4].c_str());
                tmpObject.size.h = atoi(array[5].c_str());
                tmpObject.angle = atoi(array[6].c_str());
                if (array[7] == "True")
                {
                  tmpObject.visable = true;
                }
                else
                {
                  tmpObject.visable = false;
                }
                tmpObject.texture = Render_loadImage( array[8] );
                if( tmpObject.texture == NULL )
                {
                  //printf("%s\tCan't open %s%s%s\n", KRED, KBLUE, array[7].c_str() ,KNORMAL);
                }
                else
                {
                  ObjectStack.push_back(tmpObject);
                }
              }
            }
          }
          else
          {
            //printf("Found Comment!\n");
          }
        }
    }
    ifs.close();

    Config_LoadScripts();
  }
  else
  {
    //printf("Can't open config file!\n");
    exit(1);
  }
}
std::vector<std::string> split_no_quotes (const std::string & str)
{
  std::vector<std::string> array;
  string chunk;
  bool inQuotes = false;
  for (int x = 0; x < str.size(); x++)
  {
    if (str[x] == '\"')
    {
      //printf("Found Quote!");
      inQuotes = !inQuotes;
    }
    else if (inQuotes == false && str[x] == ' ')
    {
      //printf("Pushing back chunk!");
      array.push_back(chunk);
      chunk = "";
    }
    else
    {
      chunk.push_back(str[x]);
    }
  }
  array.push_back(chunk);
  return array;
}
void Config_LoadScripts()
{
  ifstream ifs;
  string line;
  vector<string> array;
  script_t scriptObject;
  bool insideBlock = false;
  ifs.open("GaugePanel.conf");
  if (ifs.is_open())
  {
    ScriptStack.clear();
    while (getline (ifs, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end()); //Remove all newline characters
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); //Remove all carage return characters
        //printf("%sLine%s = %s\n%s", KRED, KBLUE, line.c_str(), KNORMAL);
        if (line.size() > 0)
        {
          if (line.at(0) != '#')
          {
            //array = split(line, ' ');
            array = split_no_quotes(line);
            /*for (int x = 0; x < array.size(); x++)
            {
              printf("[%d] %s\n", x, array[x].c_str());
            }*/
            if (insideBlock == false)
            {
              if (array[0] == "function")
              {
                insideBlock = true;
                scriptObject.name = array[1];
                //printf("Opening %s\n", scriptObject.name.c_str());
              }
            }
            else //if were insideBlock
            {
              if (array[0] == "end")
              {
                insideBlock = false;
                ScriptStack.push_back(scriptObject);
                //printf("Closing %s\n", scriptObject.name.c_str());
              }
              else //Populate ScriptStack
              {
                scriptObject.code.push_back(line);
                //printf("\t> %s\n", line.c_str());
              }
            }


          }
        }
    }
    ifs.close();
  }
}
void EvalCode(string code)
{
  code = std::regex_replace(code, std::regex("^ +| +$|( ) +"), "$1");
  //printf("Code = %s\n", code.c_str());
  vector<string> array;
  array = split_no_quotes(code);
  if (array[0] == "quit")
  {
    Render_Close();
    exit(0);
  }
  if (array[0] == "exec")
  {
    system(array[1].c_str());
  }
  if (array[0] == "print")
  {
    printf("%s\n", array[1].c_str());
  }
  if (array[0] == "push")
  {
    string tagname = array[1];
    string value = array[2];
    string line = tagname + " " + value + "\n";
    if (serialfd > 0) write (serialfd, line.c_str(), line.length());
  }
  if (array[0] == "set")
  {
    string tagname = array[1];
    int tag;
    if (array[2] == "Value")
    {
      for (int x = 0; x < ObjectStack.size(); x++)
      {
        if (ObjectStack[x].tagname == tagname)
        {
          tag = x;
          ObjectStack[tag].value = atof(array[3].c_str());
          if (ObjectStack[tag].type == NEEDLE)
          {
            ObjectStack[tag].angle = map_range(ObjectStack[tag].value, ObjectStack[tag].minvalue, ObjectStack[tag].maxvalue, ObjectStack[tag].minangle, ObjectStack[tag].maxangle);
          }
          Render_RenderStack();
        }
      }
    }
    if (array[2] == "Visability")
    {
      if (array[3] == "False")
      {
        int tag = GetByTag(tagname);
        if (tag > 0)
        {
          ObjectStack[tag].visable = false;
        }
      }
      if (array[3] == "True")
      {
        int tag = GetByTag(tagname);
        if (tag > 0)
        {
          ObjectStack[tag].visable = true;
        }
      }
    }
  }
}
void Config_RunScript(string tag)
{
  bool found = false;
  for (int x = 0; x < ScriptStack.size(); x++)
  {
    if (ScriptStack[x].name == tag)
    {
      found = true;
      for (int y = 0; y < ScriptStack[x].code.size(); y++)
      {
        EvalCode(ScriptStack[x].code[y]);
      }
    }
    if (found) break;
  }
}
int GetByTag(string tag)
{
  for(int x = 0; x < ObjectStack.size(); x++)
  {
    if (ObjectStack[x].tagname == tag)
    {
      return x;
    }
  }
  return -1;
}
