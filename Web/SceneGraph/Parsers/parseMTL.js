
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerParser('mtl', function(scene, assetFile, options) {

  var materialName,
    materialData = {},
    materialLibrary = options.materialLibrary ? options.materialLibrary : {},
    imageLibrary = options.imageLibrary ? options.imageLibrary : {},
    materialNames = options.materialNames;
    
  if(!options.materialType){
    options.materialType = "PhongTextureMaterial";
  }
  if(!options.materialProperties){
    options.materialProperties = {
      Ns: 'Ns',
      Ni: 'Ni',
      d: 'd',
      Tr: 'Tr',
      Tf: 'Tf',
      illum: 'illum',
      Ka: 'Ka',
      Kd: 'Kd',
      Ks: 'Ks',
      Ke: 'Ke'
    }
  }
  if(!options.materialMaps){
    options.materialMaps = {
      map_Kd: 'diffuseTexture',
      map_Ka: 'specularTexture'
    }
  }
  var materialProperties = {};
  for(var i in options.materialProperties){
    materialProperties[i.toLowerCase()] = options.materialProperties[i];
  }
  var materialMaps = {};
  for(var i in options.materialMaps){
    materialMaps[i.toLowerCase()] = options.materialMaps[i];
  }

  var onMaterialParsed = function(materialType, data, assetName) {
    var materialOptions = {
      name: assetName 
    };
    for (i in options.materialOptions) {
      materialOptions[i] = options.materialOptions[i];
    }
    for (i in materialMaps) {
      materialOptions[materialMaps[i]+'Node'] = data[materialMaps[i]];
    }
    var graphNode = scene.constructNode(materialType, materialOptions);
    materialLibrary[assetName] = graphNode;
  }


  var objText = FABRIC.loadResourceURL(assetFile, 'text/plain');
  var lines = objText.split('\n');
  if (lines.length <= 1) {
    lines = objText.split('\r');
  }

  if (lines.length <= 1) {
    if (onFailFn) {
      onFailFn('Incorect obj file encoding');
    }
    return;
  }

  var numLines = lines.length;
  var numSubAssets = 0;
  var lineNumber = 0;
  var materialType = options.materialType;
  while (lineNumber < numLines) {
    var line = lines[lineNumber].replace(/^\s+|\s+$/g, '');
    ++lineNumber;
    if (line.length <= 1 || line[0] == '#') {
      continue;
    }
    var separatorPos = line.indexOf(' ');
    var identifier = line.substring(0, separatorPos);
    var value = line.substring(separatorPos+1);

    if (identifier == 'newmtl') {
      if (materialName && (!materialNames || materialNames.indexOf(materialName) !== -1)) {
        onMaterialParsed(materialType, materialData, materialName);
      }
      materialName = value;
      materialType = options.materialType;
    }
    else if (identifier == 'shader') {
      materialType = value;
    }
    // Here we map values in the MTL file with properties and maps in the material.
    else if (materialProperties[identifier] && value) {
      if (!materialNames || materialNames.indexOf(materialName) !== -1) {
        materialData[materialProperties[identifier]] = parseFloat(value);
      }
    }
    else if (materialMaps && materialMaps[identifier.toLowerCase()] && value) {
      if (!materialNames || materialNames.indexOf(materialName) !== -1) {
        if(!imageLibrary[value]){
        //  console.log(value);
          imageLibrary[value] = scene.constructNode('Image2D', {
            name: value,
            url: options.basePath + '/' + value
          });
        }
        materialData[materialMaps[identifier.toLowerCase()]] = imageLibrary[value];
      }
    }
  }

  if (materialName && (!materialNames || materialNames.indexOf(materialName) !== -1)) {
    onMaterialParsed(materialType, materialData, materialName);
  }

  return materialLibrary;
});


