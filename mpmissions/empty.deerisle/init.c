//BBP Cement Mixers -Begin Part1-
void SpawnObject( string type, vector position, vector orientation )
{
    auto obj = GetGame().CreateObject( type, position );
    obj.SetPosition( position );
    obj.SetOrientation( orientation );
    //Force collision update
    vector roll = obj.GetOrientation();
    roll [ 2 ] = roll [ 2 ] - 1;
    obj.SetOrientation( roll );
    roll [ 2 ] = roll [ 2 ] + 1;
    obj.SetOrientation( roll );
}

void main()
{

	//INIT WEATHER BEFORE ECONOMY INIT------------------------
    Weather weather = g_Game.GetWeather();
 
    weather.MissionWeather(true);    // false = use weather controller from Weather.c
 
    weather.GetRain().SetLimits( 0.0 , 0.0 );
    weather.GetOvercast().SetLimits( 0.0 , 0.0 );
    weather.GetFog().SetLimits( 0.0 , 0.01 );
 
    weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.1 );
    weather.GetRain().SetForecastChangeLimits( 0.0, 0.0 );
    weather.GetFog().SetForecastChangeLimits( 0.0, 0.0 );
 
    weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
    weather.GetRain().SetForecastTimeLimits( 600 , 600 );
    weather.GetFog().SetForecastTimeLimits( 1800 , 1800 );
 
    weather.GetOvercast().Set( Math.RandomFloatInclusive(0.0, 0.3), 0, 0);
    weather.GetRain().Set( Math.RandomFloatInclusive(0.0, 0.0), 0, 0);
    weather.GetFog().Set( Math.RandomFloatInclusive(0.0, 0.1), 0, 0);
 
    weather.SetWindMaximumSpeed(15);
    weather.SetWindFunctionParams(0.1, 0.3, 50);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
	
     // CEApi TestHive = GetCEApi();
     // TestHive.ExportProxyProto();
     // TestHive.ExportProxyData( "8096 0 8096", 16384 );
     // TestHive.ExportClusterData() ;
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);

		GetGame().SelectPlayer(identity, m_player);

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemTop;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
			if ( Class.CastTo(itemBs, itemEnt ) )
				itemBs.SetQuantity(4);

			SetRandomHealth(itemEnt);

			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt(0, 4);
			itemEnt = itemTop.GetInventory().CreateInInventory(chemlightArray[rndIndex]);
			SetRandomHealth(itemEnt);

			rand = Math.RandomFloatInclusive(0.0, 1.0);
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory("Apple");
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory("Pear");
			else
				itemEnt = player.GetInventory().CreateInInventory("Plum");

			SetRandomHealth(itemEnt);
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}