#pragma once

#include <set>
#include <Shader.h>
#include "RenderSystem.h"

/*
*   
*/

class ResourceCache
  {
private:
  //  lists all the cached shader programs, and the set of file paths they were created from
  typedef std::pair<ShaderProgramPtr, std::set<string>> ShaderProgramAndFiles;
  std::list<ShaderProgramAndFiles> shaderPrograms;

  std::map<string, MeshStoragePtr> meshStorages;
  std::map<string, TexturePtr> textures;

public:
  ShaderProgramPtr getShaderProgram(const std::vector<mathernogl::Shader>* shaders);
  void addShaderProgram(ShaderProgramPtr shaderProgram, const std::vector<mathernogl::Shader>* shaders);
  void forEachShaderProgram(std::function<void(ShaderProgramPtr shaderProgram)> func);

  MeshStoragePtr getMeshStorage(const string& meshFilePath);
  void addMeshStorage(MeshStoragePtr meshStorage, const string& meshFilePath);
  void forEachMeshStorage(std::function<void(MeshStoragePtr meshStorage)> func);

  TexturePtr getTexture(const string& imageFilePath);
  void addTexture(TexturePtr texture, const string& imageFilePath);
  void forEachTexture(std::function<void(TexturePtr texture)> func);

  void clearAll();
  };
