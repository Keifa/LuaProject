
#include "Variables.h"

// Initiation functions
void InitTextures(sf::Texture* player, sf::Texture* box, sf::Texture* ground, sf::Texture* button);
bool InitLua(lua_State* L);
bool InitEditorLua(lua_State* L);

// Draw functions
void DrawTiles(Window& w, sf::Shape& shape, sf::Texture* texture, lua_State* L, const std::string& getEntityString, int x, int y);
void DrawEntity(Window& w, sf::Shape& shape, sf::Texture* texture, lua_State* L, const std::string& getEntityString);

// Event handling
void HandleEvents(Window& w, lua_State* L, sf::Event& event);
void HandleMouseButtonPressed(lua_State* L, sf::Event& event);
void HandleKeyPressed(lua_State* L, sf::Event& event);

//
void Update(lua_State* L);

bool editor = false;

int main()
{
	lua_State* L = luaL_newstate();
	
	bool hasLoaded = false;
	if (editor)
	{
		hasLoaded = InitEditorLua(L);
	}
	else
	{
		hasLoaded = InitLua(L);
	}

	if (hasLoaded)
	{
		Window w("window.lua");

		sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		shape.setFillColor(sf::Color::White);
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(2);

		sf::Texture* playerTexture = new sf::Texture(),
				   * boxTexture =	 new sf::Texture(),
				   * groundTexture = new sf::Texture(),
				   * buttonTexture = new sf::Texture();
		InitTextures(playerTexture, boxTexture, groundTexture, buttonTexture);

		sf::Event event;

		int size = 0;
		lua_getglobal(L, "mapSize");
		size = lua_tointeger(L, -1);

		int error = 0;

		while (w.IsOpen())
		{
			if (!editor)
			{
				Update(L);
			}
			HandleEvents(w, L, event);

			for (int y = 0; y < size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					DrawTiles(w, shape, groundTexture, L, "GetTile", x, y);
				}
			}

			DrawEntity(w, shape, buttonTexture, L, "GetButton");
			DrawEntity(w, shape, boxTexture, L, "GetBox");
			DrawEntity(w, shape, playerTexture, L, "GetPlayer");

			w.Display();
		}

		lua_close(L);
	}
	
	return 0;
}

void DrawEntity(Window& w, sf::Shape& shape, sf::Texture* texture, lua_State* L, const std::string& getEntityString)
{
	lua_getglobal(L, getEntityString.c_str());
	int error = lua_pcall(L, 0, 3, 0);
	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	else if (!error)
	{
		shape.setPosition(TILE_SIZE * lua_tointeger(L, -2), TILE_SIZE * lua_tointeger(L, -1));
		shape.setTexture(texture);
		shape.setFillColor(sf::Color::White);
		w.Draw(shape);
		lua_pop(L, 3);
	}
}

void DrawTiles(Window& w, sf::Shape& shape, sf::Texture* texture, lua_State* L, const std::string& getEntityString, int x, int y)
{
	shape.setPosition(TILE_SIZE * x, TILE_SIZE * y);

	lua_getglobal(L, "GetTile");
	lua_pushinteger(L, x + 1);
	lua_pushinteger(L, y + 1);

	int error = lua_pcall(L, 2, 1, 0);
	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	else if (!error)
	{
		int x = lua_tonumber(L, -1);
		lua_pop(L, 1);

		switch (x)
		{
		case NORMAL_GROUND:
			shape.setTexture(texture);
			shape.setFillColor(sf::Color::White);
			break;
		case WALL:
			shape.setTexture(texture);
			shape.setFillColor(sf::Color::Black);
			break;
		default:
			std::cout << "[C++] ERROR! UNVALID TILE TYPE!\n";
			break;
		}
	}
	w.Draw(shape);
}

void InitTextures(sf::Texture* player, sf::Texture* box, sf::Texture* ground, sf::Texture* button)
{
	//player = new sf::Texture();
	if (!player->loadFromFile("Player.png"))
		throw std::runtime_error("Could not load image.png");

	//box = new sf::Texture();
	if (!box->loadFromFile("Box.png"))
		throw std::runtime_error("Could not load image.png");

	//ground = new sf::Texture();
	if (!ground->loadFromFile("ground.png"))
		throw std::runtime_error("Could not load image.png");

	//button = new sf::Texture();
	if (!button->loadFromFile("button.png"))
		throw std::runtime_error("Could not load image.png");
}

bool InitLua(lua_State* L)
{
	bool check = false;
	luaL_openlibs(L);
	if (L)
	{
		RegisterEntity(L);
		int error = luaL_loadfile(L, "map.lua") ||
			lua_pcall(L, 0, 0, 0);

		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}

		error = luaL_loadfile(L, "handleLevelChange.lua") ||
			lua_pcall(L, 0, 0, 0);
		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		check = true;
	}
	return check;
}

bool InitEditorLua(lua_State* L)
{
	bool check = false;
	luaL_openlibs(L);
	if (L)
	{
		RegisterEntity(L);
		int error = luaL_loadfile(L, "editor.lua") ||
			lua_pcall(L, 0, 0, 0);

		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		check = true;
	}
	return check;
}

void HandleEvents(Window& w, lua_State* L, sf::Event& event)
{
	while (w.PollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			w.Close();
			break;
		case sf::Event::KeyPressed:
			HandleKeyPressed(L, event);
			break;
		case sf::Event::MouseButtonPressed:
			HandleMouseButtonPressed(L, event);
			break;
		}
	}
}

void HandleMouseButtonPressed(lua_State* L, sf::Event& event)
{
	lua_getglobal(L, "Clicked");
	lua_pushinteger(L, event.mouseButton.x / TILE_SIZE + 1);
	lua_pushinteger(L, event.mouseButton.y / TILE_SIZE + 1);
	int error = lua_pcall(L, 2, 0, 0);
	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

void HandleKeyPressed(lua_State* L, sf::Event& event)
{
	std::string tempStr = keyPressMap[event.key.code];
	if (editor)
	{
		tempStr = editorKeyPressMap[event.key.code];
	}
	if (tempStr != "")
	{
		lua_getglobal(L, "HandleKeyPress");
		lua_pushstring(L, tempStr.c_str());
		int error = lua_pcall(L, 1, 0, 0);
		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else
		std::cout << "Invalid input\n";
}

void Update(lua_State* L)
{
	lua_getglobal(L, "CheckLevelDone");
	int error = lua_pcall(L, 0, 0, 0);
	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}