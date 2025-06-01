#include "Texture.hpp"

#include "Renderer.hpp"
#include "Utils.hpp"
#include "Renderable.hpp"
#include "Input.hpp"
#include "Mesh.hpp"

#ifndef HEADLESS
#include <GL/glew.h>
#include <SDL2/SDL_image.h>

enum class TextureWrap
{
	Clamp,
	Repeat,
};

enum class TextureMinify
{
	Nearest,
	Linear,
	NearestNearest,
	NearestLinear,
	LinearNearest,
	LinearLinear,
};

enum class TextureMagnify
{
	Nearest,
	Linear,
};

static std::vector<Uint> textures;

static Uint uploadTextureRGBA(int w, int h, unsigned char *data, TextureWrap wrap, TextureMinify minify, TextureMagnify magnify)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	switch (wrap)
	{
		case TextureWrap::Clamp:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case TextureWrap::Repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
	}
	switch (minify)
	{
		case TextureMinify::Nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		case TextureMinify::Linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case TextureMinify::NearestNearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			break;
		case TextureMinify::NearestLinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case TextureMinify::LinearNearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			break;
		case TextureMinify::LinearLinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
	}
	switch (magnify)
	{
		case TextureMagnify::Nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TextureMagnify::Linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	if (minify != TextureMinify::Nearest && minify != TextureMinify::Linear) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	textures.push_back(texture);
	return texture;
}

static Mesh meshVer, meshHor;

static GLuint shaderSprite;
static GLint uniformSpriteProj, uniformSpriteView, uniformSpriteModel, uniformSpriteOffset, uniformSpriteLightStrength, uniformSpritePointLightSum, uniformSpriteOpacity;

static GLuint shaderSpriteShadow;
static GLint uniformSpriteShadowProj, uniformSpriteShadowView, uniformSpriteShadowModel, uniformSpriteShadowOffset, uniformSpriteShadowShadowOffset, uniformSpriteShadowLight, uniformSpriteShadowOpacity;

static GLuint shaderGrid;
static GLint uniformGridProj, uniformGridView, uniformGridScale, uniformGridOffset, uniformGridLightStrength, uniformGridPointLights, uniformGridPointLightsCount;

static GLuint shaderTerrain;
static GLint uniformTerrainProj, uniformTerrainView, uniformTerrainLightStrength;

static GLuint shaderUISprite;
static GLint uniformUISpriteProj, uniformUISpritePos, uniformUISpriteSize;

static GLuint shaderUIBox;
static GLint uniformUIBoxProj, uniformUIBoxPos, uniformUIBoxSize, uniformUIBoxColor, uniformUIBoxBorderSize, uniformUIBoxBorderColor;

static Mat4 projUI;

static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	(void) source;
	(void) type;
	(void) id;
	(void) severity;
	(void) length;
	(void) userParam;
	logDebug("GL DEBUG: %s", message);
}

static void calcProjUI()
{
	const Ivec2 size = Input::getWindowSize();
	projUI = glm::ortho(0.0f, (float) size.x, (float) size.y, 0.0f, 1000.0f, -1000.0f);
}

static GLuint openShaderSingle(const std::string &path, GLenum type)
{
	assert(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);
	const std::string source = readTextFile(path);
	const GLuint shader = glCreateShader(type);
	const char *source_cstr = source.c_str();
	glShaderSource(shader, 1, &source_cstr, nullptr);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		GLsizei length;
		glGetShaderInfoLog(shader, sizeof (log), &length, log);
		logFatal("Shader Error (%s): %.*s", path.c_str(), (int) length, log);
	}
	return shader;
}

static GLuint openShader(const std::string &name)
{
	const std::string path_vert = DIR_SHADERS + name + ".vert";
	const std::string path_frag = DIR_SHADERS + name + ".frag";
	const GLuint shaderVert = openShaderSingle(path_vert, GL_VERTEX_SHADER);
	const GLuint shaderFrag = openShaderSingle(path_frag, GL_FRAGMENT_SHADER);
	const GLuint shader = glCreateProgram();
	glAttachShader(shader, shaderVert);
	glAttachShader(shader, shaderFrag);
	glLinkProgram(shader);
	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		GLsizei length;
		glGetProgramInfoLog(shader, sizeof (log), &length, log);
		logFatal("Program Error (%s): %.*s", name.c_str(), (int) length, log);
	}
	glDeleteShader(shaderVert);
	glDeleteShader(shaderFrag);
	return shader;
}

void Renderer::init()
{
	assert(glewInit() == GLEW_OK);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1.0f, 1.0f, 1.0, 1.0f);

	const std::vector<Vertex> veticesHor = {
		{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	};
	meshHor = Mesh {veticesHor};

	const std::vector<Vertex> veticesVer = {
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	};
	meshVer = Mesh {veticesVer};

	shaderSprite = openShader("sprite");
	uniformSpriteProj = glGetUniformLocation(shaderSprite, "proj");
	assert(uniformSpriteProj != -1);
	uniformSpriteView = glGetUniformLocation(shaderSprite, "view");
	assert(uniformSpriteView != -1);
	uniformSpriteModel = glGetUniformLocation(shaderSprite, "model");
	assert(uniformSpriteModel != -1);
	uniformSpriteOffset = glGetUniformLocation(shaderSprite, "offset");
	assert(uniformSpriteOffset != -1);
	uniformSpriteLightStrength = glGetUniformLocation(shaderSprite, "lightStrength");
	assert(uniformSpriteLightStrength != -1);
	uniformSpritePointLightSum = glGetUniformLocation(shaderSprite, "pointLightSum");
	assert(uniformSpritePointLightSum != -1);
	uniformSpriteOpacity = glGetUniformLocation(shaderSprite, "opacity");
	assert(uniformSpriteOpacity != -1);

	shaderSpriteShadow = openShader("sprite-shadow");
	uniformSpriteShadowProj = glGetUniformLocation(shaderSpriteShadow, "proj");
	assert(uniformSpriteShadowProj != -1);
	uniformSpriteShadowView = glGetUniformLocation(shaderSpriteShadow, "view");
	assert(uniformSpriteShadowView != -1);
	uniformSpriteShadowModel = glGetUniformLocation(shaderSpriteShadow, "model");
	assert(uniformSpriteShadowModel != -1);
	uniformSpriteShadowOffset = glGetUniformLocation(shaderSpriteShadow, "offset");
	assert(uniformSpriteShadowOffset != -1);
	uniformSpriteShadowShadowOffset = glGetUniformLocation(shaderSpriteShadow, "shadowOffset");
	assert(uniformSpriteShadowShadowOffset != -1);
	uniformSpriteShadowLight = glGetUniformLocation(shaderSpriteShadow, "light");
	assert(uniformSpriteShadowLight != -1);
	uniformSpriteShadowOpacity = glGetUniformLocation(shaderSpriteShadow, "opacity");
	assert(uniformSpriteShadowOpacity != -1);

	shaderGrid = openShader("grid");
	uniformGridProj = glGetUniformLocation(shaderGrid, "proj");
	assert(uniformGridProj != -1);
	uniformGridView = glGetUniformLocation(shaderGrid, "view");
	assert(uniformGridView != -1);
	uniformGridScale = glGetUniformLocation(shaderGrid, "scale");
	assert(uniformGridScale != -1);
	uniformGridOffset = glGetUniformLocation(shaderGrid, "offset");
	assert(uniformGridOffset != -1);
	uniformGridLightStrength = glGetUniformLocation(shaderGrid, "lightStrength");
	assert(uniformGridLightStrength != -1);
	uniformGridPointLights = glGetUniformLocation(shaderGrid, "pointLights");
	assert(uniformGridPointLights != -1);
	uniformGridPointLightsCount = glGetUniformLocation(shaderGrid, "pointLightsCount");
	assert(uniformGridPointLightsCount != -1);

	shaderTerrain = openShader("terrain");
	uniformTerrainProj = glGetUniformLocation(shaderTerrain, "proj");
	assert(uniformTerrainProj != -1);
	uniformTerrainView = glGetUniformLocation(shaderTerrain, "view");
	assert(uniformTerrainView != -1);
	uniformTerrainLightStrength = glGetUniformLocation(shaderTerrain, "lightStrength");
	assert(uniformTerrainLightStrength != -1);

	shaderUISprite = openShader("ui-sprite");
	uniformUISpriteProj = glGetUniformLocation(shaderUISprite, "uni_proj");
	assert(uniformUISpriteProj != -1);
	uniformUISpritePos = glGetUniformLocation(shaderUISprite, "uni_pos");
	assert(uniformUISpritePos != -1);
	uniformUISpriteSize = glGetUniformLocation(shaderUISprite, "uni_size");
	assert(uniformUISpriteSize != -1);

	shaderUIBox = openShader("ui-box");
	uniformUIBoxProj = glGetUniformLocation(shaderUIBox, "uni_proj");
	assert(uniformUIBoxProj != -1);
	uniformUIBoxPos = glGetUniformLocation(shaderUIBox, "uni_pos");
	assert(uniformUIBoxPos != -1);
	uniformUIBoxSize = glGetUniformLocation(shaderUIBox, "uni_size");
	assert(uniformUIBoxSize != -1);
	uniformUIBoxColor = glGetUniformLocation(shaderUIBox, "uni_color");
	assert(uniformUIBoxColor != -1);
	uniformUIBoxBorderSize = glGetUniformLocation(shaderUIBox, "uni_border_size");
	assert(uniformUIBoxBorderSize != -1);
	uniformUIBoxBorderColor = glGetUniformLocation(shaderUIBox, "uni_border_color");
	assert(uniformUIBoxBorderColor != -1);

	calcProjUI();

}

void Renderer::render()
{

	if (Input::didWindowSizeChange()) {
		const Ivec2 size = Input::getWindowSize();
		glViewport(0, 0, size.x, size.y);
		calcProjUI();
	}

	const auto &viewPoint = Renderable::getUnique<Renderable::ViewPoint>();
	const auto &lightGlobal = Renderable::getUnique<Renderable::LightGlobal>();
	const auto &background = Renderable::getUnique<Renderable::Background>();

	// TODO: make proper lights class and avoid copies
	std::vector<Vec4> pointLights;
	Renderable::visitAll<Renderable::LightPoint>([&pointLights](const Renderable::LightPoint &input) {
		const Vec4 light = {*input.getPosition(), input.getRadius()};
		pointLights.push_back(light);
		// TODO: optimize or fail earlier
		if (pointLights.size() > 512) {
			logFatal("too many point lights");
		}
	});

	const Vec3 backgroundLighted = background.getColor() * lightGlobal.getStrength();
	glClearColor(backgroundLighted.r, backgroundLighted.g, backgroundLighted.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderGrid);
	glUniformMatrix4fv(uniformGridProj, 1, GL_FALSE, glm::value_ptr(viewPoint.proj));
	glUniformMatrix4fv(uniformGridView, 1, GL_FALSE, glm::value_ptr(viewPoint.view));
	glUniform1f(uniformGridLightStrength, lightGlobal.getStrength());
	glUniform4fv(uniformGridPointLights, (int) pointLights.size(), (const float *) pointLights.data());
	glUniform1i(uniformGridPointLightsCount, (int) pointLights.size());

	Renderable::visitAll<Renderable::Grid>([&pointLights](const Renderable::Grid &grid) {
		glUniform1f(uniformGridScale, grid.scale);
		for (int x = 0; x < grid.size.x; ++x) {
			for (int z = 0; z < grid.size.y; ++z) {
				const auto &texture = grid.at({x, z});
				if (texture) {
					glBindTexture(GL_TEXTURE_2D, texture->getTexture());
					const Vec3 offset = grid.pos + Vec3{x, 0, z} * grid.scale;
					glUniform3f(uniformGridOffset, offset.x, offset.y, offset.z);
					meshHor.render();
				}
			}
		}
	});

	glUseProgram(shaderTerrain);
	glUniformMatrix4fv(uniformTerrainProj, 1, GL_FALSE, glm::value_ptr(viewPoint.proj));
	glUniformMatrix4fv(uniformTerrainView, 1, GL_FALSE, glm::value_ptr(viewPoint.view));
	glUniform1f(uniformTerrainLightStrength, lightGlobal.getStrength());

	Renderable::visitAll<Renderable::Terrain>([&pointLights](const Renderable::Terrain &terrain) {
		glBindTexture(GL_TEXTURE_2D, terrain.texture.getTexture());
		terrain.mesh.render();
	});

	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaderSpriteShadow);
	glUniformMatrix4fv(uniformSpriteShadowProj, 1, GL_FALSE, glm::value_ptr(viewPoint.proj));
	glUniformMatrix4fv(uniformSpriteShadowView, 1, GL_FALSE, glm::value_ptr(viewPoint.view));
	glUniform4f(uniformSpriteShadowLight, lightGlobal.getDirection().x, lightGlobal.getDirection().y, lightGlobal.getDirection().z, lightGlobal.getStrength());
	Renderable::visitAll<Renderable::Sprite>([](const Renderable::Sprite &sprite) {
		glBindTexture(GL_TEXTURE_2D, sprite.getTexture().getTexture());
		glUniformMatrix4fv(uniformSpriteShadowModel, 1, GL_FALSE, glm::value_ptr(*sprite.getTransform()));
		glUniform1f(uniformSpriteShadowOpacity, sprite.getOpacity());
		glUniform3fv(uniformSpriteShadowOffset, 1, glm::value_ptr(sprite.getOffset()));
		glUniform1f(uniformSpriteShadowShadowOffset, sprite.shadowOffset);
		meshVer.render();
	});

	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderSprite);
	glUniformMatrix4fv(uniformSpriteProj, 1, GL_FALSE, glm::value_ptr(viewPoint.proj));
	glUniformMatrix4fv(uniformSpriteView, 1, GL_FALSE, glm::value_ptr(viewPoint.view));
	glUniform1f(uniformSpriteLightStrength, lightGlobal.getStrength());
	Renderable::visitAll<Renderable::Sprite>([&pointLights](const Renderable::Sprite &sprite) {
		float pointLightSum = 0.0f;
		for (Vec4 pointLight : pointLights) {
			const Vec3 lightPosition = Vec3 {pointLight};
			const float lightRadius = pointLight.w;
			const float dist = glm::length(*sprite.getPosition() - lightPosition);
			if (dist <= lightRadius) {
				const float value = 1.0f - glm::smoothstep(0.0f, lightRadius, dist);
				pointLightSum += value;
			}
		}
		pointLightSum = glm::clamp(pointLightSum, 0.0f, 1.0f);
		glUniform1f(uniformSpritePointLightSum, pointLightSum);
		glUniform1f(uniformSpriteOpacity, sprite.getOpacity());
		glUniformMatrix4fv(uniformSpriteModel, 1, GL_FALSE, glm::value_ptr(*sprite.getTransform()));
		glUniform3fv(uniformSpriteOffset, 1, glm::value_ptr(sprite.getOffset()));
		glBindTexture(GL_TEXTURE_2D, sprite.getTexture().getTexture());
		meshVer.render();
	});

	// TODO: one shader ?
	glDisable(GL_DEPTH_TEST);
	Renderable::visitAll<Renderable::UI>([](const Renderable::UI &ui) {
		const auto visitor = Overload
		{
			[](Renderable::UI::Box *box)
			{
				if (box->hidden) return;
				const Ivec3 pos = Ivec3{box->pos, box->level};
				glUseProgram(shaderUIBox);
				glUniformMatrix4fv(uniformUIBoxProj, 1, GL_FALSE, glm::value_ptr(projUI));
				glUniform3iv(uniformUIBoxPos, 1, glm::value_ptr(pos));
				glUniform2iv(uniformUIBoxSize, 1, glm::value_ptr(box->size));
				glUniform4fv(uniformUIBoxColor, 1, glm::value_ptr(box->color));
				glUniform4iv(uniformUIBoxBorderSize, 1, glm::value_ptr(box->borderSize));
				glUniform4fv(uniformUIBoxBorderColor, 1, glm::value_ptr(box->borderColor));
				meshVer.render();
			},
			[](Renderable::UI::Sprite *sprite)
			{
				if (sprite->hidden) return;
				const Ivec3 pos = Ivec3{sprite->pos, sprite->level};
				glUseProgram(shaderUISprite);
				glUniformMatrix4fv(uniformUISpriteProj, 1, GL_FALSE, glm::value_ptr(projUI));
				glUniform3iv(uniformUISpritePos, 1, glm::value_ptr(pos));
				glUniform2iv(uniformUISpriteSize, 1, glm::value_ptr(sprite->size));
				glBindTexture(GL_TEXTURE_2D, sprite->texture.getTexture());
				meshVer.render();
			},
			[](Renderable::UI::Text *text)
			{
				if (text->hidden) return;
				assert(false);
			},
		};
		for (const auto &input : ui.inputs) {
			std::visit(visitor, input);
		}
	});
	glEnable(GL_DEPTH_TEST);

}

void Renderer::destroy()
{
	for (Uint texture : textures) {
		glDeleteTextures(1, &texture);
	}
	meshVer.destroy();
	meshHor.destroy();
}

#endif // HEADLESS

void Texture::destroy()
{
#ifndef HEADLESS
	glDeleteTextures(1, &texture);
	std::erase(textures, texture);
#endif // HEADLESS
}

Uint Texture::textureFromImage(const std::string &name)
{
#ifndef HEADLESS
	const std::string path = DIR_SPRITES + name;
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (!surface) {
		logFatal("failed to load texture: %s", path.c_str());
	}
	if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
		SDL_Surface *surfaceRGBA = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		assert(surfaceRGBA);
		SDL_FreeSurface(surface);
		surface = surfaceRGBA;
	}
	const Uint texture = uploadTextureRGBA(surface->w, surface->h, (unsigned char *) surface->pixels, TextureWrap::Repeat, TextureMinify::NearestNearest, TextureMagnify::Nearest);
	SDL_FreeSurface(surface);
	return texture;
#else
	return {};
#endif // HEADLESS
}

Uint Texture::textureFromBitmapRGBA(Uint w, Uint h, const void *pixels)
{
#ifndef HEADLESS
	return uploadTextureRGBA(w, h, (unsigned char *) pixels, TextureWrap::Repeat, TextureMinify::Nearest, TextureMagnify::Nearest);
#else
	return {};
#endif // HEADLESS

}

Mesh::Mesh(const std::vector<Vertex> &vertices)
{
#ifndef HEADLESS
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), reinterpret_cast<void *>(0 * sizeof (float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), reinterpret_cast<void *>(3 * sizeof (float)));
	glEnableVertexAttribArray(1);
	count = (Uint) vertices.size();
#endif // HEADLESS
}

void Mesh::destroy()
{
#ifndef HEADLESS
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
#endif // HEADLESS
}

void Mesh::render() const
{
#ifndef HEADLESS
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, count);
#endif // HEADLESS
}
