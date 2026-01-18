#include <_a_autoupdate.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;

inline static std::string server = Mod::get()->getDescription().value_or("retray.bccst.ru");

inline static auto links = matjson::parse(R"({
    "asdasd": "asdasd",
    "https://www.robtopgames.com": "https://t.me/thegmdguschin",
    "https://www.boomlings.com/database/accounts/accountManagement.php": "https://retray.bccst.ru/dashboard/",
    "https://discord.com/invite/geometrydash": "https://discord.gg/pP5T6WTJdS",
    "https://twitter.com/robtopgames": "https://t.me/retraynet",
    "https://www.youtube.com/user/RobTopGames": "https://www.youtube.com/@GmdGuschin"
})").unwrapOrDefault();

#include <Geode/modify/FLAlertLayer.hpp>
class $modify(FLAlertLayerExt, FLAlertLayer) {
	bool init(
		FLAlertLayerProtocol * delegate, char const* title, gd::string desc,
		char const* btn1, char const* btn2, float width,
		bool scroll, float height, float textScale
	) {
		if (!FLAlertLayer::init(
			delegate, title, desc, btn1, btn2, width, scroll, height, textScale
		)) return false;
		if (m_mainLayer) findFirstChildRecursive<CCLabelBMFont>(
			m_mainLayer, [](CCLabelBMFont* node) {
				if (node->getFntFile() == std::string("goldFont.fnt"))
					node->setFntFile("blueFont.fnt");
				return false;
			}
		);
		for (auto b : { m_button1, m_button2 }) if (b) if (auto bg = b->m_BGSprite) {
			auto size = bg->getContentSize();
			bg->setSpriteFrame(CCSprite::create("blueButton.png")->displayFrame());
			bg->setContentSize(size);
		};
		return true;
	};
};



#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayerExt, LoadingLayer) {
	bool init(bool refresh) {
		if (!LoadingLayer::init(refresh)) return false;

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("bg-texture"))) {
			a->setColor({ 22, 79, 52 });
			a->setDisplayFrame(CCSprite::create("game_bg_20_001.png")->displayFrame());
		}

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("gd-logo"))) {
			a->setPositionX(75.F);
			a->setPositionY(this->getContentHeight() - (320.F - 290.F));
			a->setScale(0.600f);
		}

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("robtop-logo"))) {
			a->setPosition(this->getContentSize() / 2);
			a->setDisplayFrame(CCSprite::create("ReBanner.png")->displayFrame());
			limitNodeHeight(a, 175.000f, 999.f, 0.f);
			//retray.bccst.ru
			Ref loader = LazySprite::create(a->getContentSize(), 0);
			loader->setPosition(a->getContentSize() / 2);
			loader->loadFromUrl("https://" + server + "/frames/frame01.png");
			
			
			CCScheduler::get()->setTimeScale(0.f);
			loader->setLoadCallback(
				[](Result<> res) {
					CCScheduler::get()->setTimeScale(1.f);
				}
			);
			a->addChild(loader);
		}

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("cocos2d-logo"))) {
			a->setPositionX(this->getContentWidth() - (569.000 - 533.000));
			a->setPositionY(this->getContentHeight() - (320.000 - 300.000));
		}

		if (Ref a = typeinfo_cast<CCSprite*>(querySelector("fmod-logo"))) {
			a->setPositionX(this->getContentWidth() - (569.000 - 541.000));
			a->setPositionY(this->getContentHeight() - (320.000 - 271.000));
			a->setColor(ccWHITE);
		}

		if (Ref a = typeinfo_cast<CCNode*>(querySelector("geode-small-label"))) {
			a->setPositionY(44.000f);
		}

		if (Ref a = typeinfo_cast<CCNode*>(querySelector("geode-small-label-2"))) {
			a->setPositionY(11.000f);
		}

		if (Ref a = typeinfo_cast<CCNode*>(querySelector("progress-slider"))) {
			a->setPositionY(28.000f);
		}

		return true;
	}
};







//send
#include <Geode/modify/CCHttpClient.hpp>
class $modify(CCHttpClientLinksReplace, CCHttpClient) {
	void send(CCHttpRequest * req) {
		std::string url = req->getUrl();
		url = string::replace(url, "www.boomlings.com/database", server);
		url = string::replace(url, "boomlings.com/database", server);
		req->setUrl(url.c_str());
		return CCHttpClient::send(req);
	}
};

//url open
#include <Geode/modify/CCApplication.hpp>
class $modify(CCApplicationLinksReplace, CCApplication) {
	$override void openURL(const char* url) {
		url = not links.contains(url) ? url : links[url].asString().unwrapOr(url).c_str();
		url = string::replace(url, "https://www.twitter.com/", "https://t.me/@").c_str();
		url = string::replace(url, "www.boomlings.com/database", server).c_str();
		url = string::replace(url, "boomlings.com/database", server).c_str();
		//log::debug("{}.url = {}", __FUNCTION__, url);
		return CCApplication::openURL(url);
	}
};

#include <Geode/modify/CCLabelBMFont.hpp>

class $modify(CCLabelBMFontExt, CCLabelBMFont) {
	bool initWithString(
		const char* str,
		const char* fntFile,
		float width = kCCLabelAutomaticWidth,
		CCTextAlignment alignment = kCCTextAlignmentLeft,
		CCPoint imageOffset = CCPointZero
	) {
		if (!str)
			return CCLabelBMFont::initWithString(str, fntFile, width, alignment, imageOffset);

		std::string label = str;

		
		label = string::replace(label, "Twitter:", "Telegram:");

		label = string::replace(label, "twitter.com/...", "t.me/...");
		label = string::replace(label, "server opened!", "Добро пожаловать!");


		log::debug("\"{}\"", str);
		return CCLabelBMFont::initWithString(
			label.c_str(), fntFile, width, alignment, imageOffset
		);
	}
};






#include <Geode/modify/GManager.hpp>
class $modify(GManagerExt, GManager) {
	void setup() {
		auto compare = std::string(m_fileName);
		compare.insert(std::string_view(compare).find(".dat"), "ReTraY");
		m_fileName = compare;
		GManager::setup();
	}
};


#pragma once
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/Geode.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

int getFrameIcon(IconType type) {
	auto gManager = GameManager::sharedState();
	switch (type) {
	default: return gManager->getPlayerFrame();
	case IconType::Ship: return gManager->getPlayerShip();
	case IconType::Ball: return gManager->getPlayerBall();
	case IconType::Ufo: return gManager->getPlayerBird();
	case IconType::Wave: return gManager->getPlayerDart();
	case IconType::Robot: return gManager->getPlayerRobot();
	case IconType::Spider: return gManager->getPlayerSpider();
	case IconType::Swing: return gManager->getPlayerSwing();
	case IconType::Jetpack: return gManager->getPlayerJetpack();
	}
}

class $modify(MenuLayer) {
public:
	bool init() {
		if (!MenuLayer::init())
			return false;

		auto manager = GameManager::sharedState();
		if (!manager) return true;

	
		auto profileMenu = this->getChildByID("profile-menu");
		if (!profileMenu) return true;


		SimplePlayer* playerIcon = SimplePlayer::create(0);
		playerIcon->updatePlayerFrame(getFrameIcon(manager->m_playerIconType), manager->m_playerIconType);

	
		playerIcon->setColor(manager->colorForIdx(manager->getPlayerColor()));
		playerIcon->setSecondColor(manager->colorForIdx(manager->getPlayerColor2()));
		playerIcon->setGlowOutline(manager->colorForIdx(manager->getPlayerGlowColor()));
		playerIcon->enableCustomGlowColor(manager->colorForIdx(manager->getPlayerGlowColor()));
		if (!manager->getPlayerGlow()) playerIcon->disableGlowOutline();

		playerIcon->setScale(1.15f);




		auto profileBtn = as<CCMenuItemSpriteExtra*>(profileMenu->getChildByID("profile-button"));
		if (profileBtn && profileBtn->getChildren()->count() > 0) {
			auto profileSpr = static_cast<cocos2d::CCSprite*>(profileBtn->getChildren()->objectAtIndex(0));
			if (profileSpr) {
				profileSpr->setID("profile-icon");
				profileSpr->setDisplayFrame(playerIcon->displayFrame());
				profileSpr->addChild(playerIcon);
				profileBtn->setPositionX(30.f);
			}
		}

		
		auto sideSprite = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");

		auto sideButton = CCMenuItemSpriteExtra::create(
			sideSprite,
			this,
			menu_selector(ProfilePage::onSettings)
		);

		sideButton->setID("profile-side-button");

	
		sideButton->setPosition(
			profileBtn->getPosition() + CCPoint{ 50.f, 0.f } 
			
		);

		
		profileMenu->addChild(sideButton, 1000);
		sideButton->setVisible(0);
		return true;
	}
};









#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerExt, MenuLayer) {
	static auto onModify(auto) {
		CCTexturePack xd;
		xd.m_id = Mod::get()->getID();
		xd.m_paths.push_back(string::pathToString(
			Mod::get()->getResourcesDir().parent_path()
		).c_str());
		log::debug("Adding texture pack: {}", xd.m_paths[0]);
		CCFileUtils::get()->addTexturePack(xd);
	}



	static cocos2d::CCScene* scene(bool isVideoOptionsOpen) {
		if (!isVideoOptionsOpen) {

			static int notJustLaunched = false;
			if (notJustLaunched++) return MenuLayer::scene(isVideoOptionsOpen);
			FMODAudioEngine::get()->playEffect("achievement_01.ogg");

			auto issues = std::vector<std::string>();
			for (auto dep : getMod()->getMetadataRef().getDependencies()) {
				if (not Loader::get()->isModLoaded(dep.id)) {
					issues.push_back(dep.id);
				}
			};
			if (!issues.size()) return MenuLayer::scene(isVideoOptionsOpen);

			GameManager::get()->fadeInMusic(".aw");

			auto stream = std::stringstream() <<
				"You should make sure that "
				"the following mods will be loaded"
				"in order to play "
				<< fmt::format(
					"[{}](mod:{})",
					getMod()->getName(),
					getMod()->getID()
				) << ":\n\n";
			for (auto dep : issues) {
				stream << "- " << fmt::format("[{}](mod:{})\n", dep, dep);
			}

			auto popup = MDPopup::create(
				"Dependencies", stream.str(), "Restart", nullptr, [](bool) {
					game::restart(true);
				}
			);
			popup->setOpacity(0);
			if (Ref a = popup->m_mainLayer->getChildByType<CCScale9Sprite>(0))
				a->setOpacity(0);
			popup->addChild(geode::createLayerBG(), -1);
			popup->addChildAtPosition(
				CCParticleSystemQuad::create("glitterEffect.plist", 0)

				, Anchor::Center, {}, !"NO LAYOUT"
			);
			addSideArt(popup);
			Ref sc = CCScene::create();
			sc->addChild(popup, 1, "popup"_h);
			sc->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
				CCDelayTime::create(.01f), CallFuncExt::create(
					[sc] {
						auto particle = sc->getChildByTag("mouse-particle"_h);
						if (!particle) {
							particle = CCParticleSystemQuad::create("keyEffect.plist", 0);
							sc->addChild(particle, INT_MAX, "mouse-particle"_h);
						}
						particle->setPosition(getMousePos());
						if (!sc->getChildByTag("popup"_h)) game::restart(true);
					}
				)
			)));
			CCScheduler::get()->setTimeScale(0.25f);
			return sc;
		} //!isVideoOptionsOpen
		return MenuLayer::scene(isVideoOptionsOpen);

	};




};