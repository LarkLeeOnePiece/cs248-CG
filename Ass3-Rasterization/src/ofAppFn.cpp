/*
Author: Da Li
Some functions used by class ofApp I implemented
*/
#include "ofApp.h"
#include "matf4x4.h"
#include"utilsFn.h"
// cpp_compiler_options_openmp.cpp
#include <omp.h>

//--------------------------------------------------------------
void ofApp::loadModel(const char* filename, const char* basepath, bool triangulate) {

    double start = omp_get_wtime();
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename,
        basepath, triangulate);


    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }

    if (!ret) {
        printf("Failed to load/parse .obj.\n");
        return;
    }
    double finish = omp_get_wtime();
    //cout << "finish:" << finish << endl;
    double elapsed = finish - start;
    cout << "loadModel: Loaded " + std::string(filename) + " took " << elapsed << "s" << endl;
    //PrintInfo(attrib, shapes, materials);
    ConvertStruct();

}
void ofApp::InfoOBJ() {
    std::cout << "# of vertices  : " << OBJData.vectices.size() << std::endl;
    std::cout << "# of normals   : " << OBJData.normals.size() << std::endl;
    std::cout << "# of texcoords : " << OBJData.textures.size() << std::endl;
    std::cout << "# of shapes    : " << OBJData.shapes.size() << std::endl;
    
    for (size_t v = 0; v < OBJData.vectices.size(); v++) {
        printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
            OBJData.vectices[v].x,
            OBJData.vectices[v].y,
            OBJData.vectices[v].z);

    }
    for (size_t v = 0; v < OBJData.normals.size(); v++) {
        printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
            OBJData.normals[v].x,
            OBJData.normals[v].y,
            OBJData.normals[v].z);
    }
    for (size_t v = 0; v < OBJData.textures.size(); v++) {
        printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
            OBJData.textures[v].u,
            OBJData.textures[v].v);
    }
    // For each shape
    for (size_t i = 0; i < OBJData.shapes.size(); i++) {
        printf("Size of shape[%ld].mesh.faces: %lu\n", static_cast<long>(i),
            OBJData.shapes[i].num_faces);
        // For each face
        for (size_t f = 0; f < OBJData.shapes[i].num_faces; f++) {
            size_t fnum = OBJData.shapes[i].faces[f].indices.size();//the num of vertices for each face
            printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
                static_cast<unsigned long>(fnum));

            // For each vertex in the face
            for (size_t v = 0; v < fnum; v++) {
                printf("  v[%ld] = (%d, %d, %d)\n", static_cast<long>(v),
                    OBJData.shapes[i].faces[f].indices[v].x,
                    OBJData.shapes[i].faces[f].indices[v].y,
                    OBJData.shapes[i].faces[f].indices[v].z);
            }
        }
    }

}

bool ofApp::ConvertStruct() {
    cout << "Converting...." << endl;
    f3Vector Modelcenter;
    OBJData.model_vectices.clear();
    OBJData.normals.clear();
    OBJData.normals_vertices.clear();
    OBJData.shapes.clear();
    OBJData.textures.clear();
    OBJData.vectices.clear();
    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        OBJData.vectices.push_back(f3Vector(attrib.vertices[3 * v + 0], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]));
        Modelcenter.x = Modelcenter.x + OBJData.vectices[v].x;
        Modelcenter.y = Modelcenter.y + OBJData.vectices[v].y;
        Modelcenter.z = Modelcenter.z + OBJData.vectices[v].z;
    }
    cout << "Modelcenter:" << " ";
    Modelcenter.x=Modelcenter.x / (attrib.vertices.size() / 3);
    Modelcenter.y = Modelcenter.y / (attrib.vertices.size() / 3);
    Modelcenter.z = Modelcenter.z / (attrib.vertices.size() / 3);
    Modelcenter.info();
    for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
        OBJData.normals.push_back(f3Vector(attrib.normals[3 * v + 0], attrib.normals[3 * v + 1], attrib.normals[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
        OBJData.textures.push_back(f2Vector(attrib.texcoords[2 * v + 0], attrib.texcoords[2 * v + 1]));
    }

    // For each shape
    for (size_t i = 0; i < shapes.size(); i++) {

        size_t index_offset = 0;

        assert(shapes[i].mesh.num_face_vertices.size() ==
            shapes[i].mesh.material_ids.size());

        assert(shapes[i].mesh.num_face_vertices.size() ==
            shapes[i].mesh.smoothing_group_ids.size());
        MESH newmesh;
        newmesh.num_faces = shapes[i].mesh.num_face_vertices.size();


        // For each face
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = shapes[i].mesh.num_face_vertices[f];
            FACES newface;
            newface.num_face_vertices = fnum;// for this faces it has fnum vertices

            // For each vertex in the face
            for (size_t v = 0; v < fnum; v++) {
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];

                newface.indices.push_back(Int3Vector(idx.vertex_index, idx.normal_index, idx.texcoord_index));

            }
            newmesh.faces.push_back(newface);

            index_offset += fnum;
        }
        OBJData.shapes.push_back(newmesh);

    }
    cout << "Converted!" << endl;
    return true;
}


/*
input: the origin of modelCoor, ArcCoor and CamCoor

*/
void ofApp::setupScene(f3Vector model_cen,f3Vector model_e, f3Vector arc_e, f3Vector cam_e,f3Vector camBox_low, f3Vector camBox_high, f3Vector light) {
    _axis_x = f3Vector(1, 0, 0);
    _axis_y = f3Vector(0, 1, 0);
    _axis_z = f3Vector(0, 0, 1);
    _Model_e = model_e;
    _Arc_e = arc_e;
    _Cam_e = cam_e;
    _CamBox_low = camBox_low;
    _CamBox_high = camBox_high;
    ofModelCoor.setup(_Model_e, _axis_x, _axis_y, _axis_z);
    ofArcCoor.setup(_Arc_e, _axis_x, _axis_y, _axis_z);
    ofCamCoor.setup(_Cam_e, _axis_x, _axis_y, _axis_z, _CamBox_low, _CamBox_high);
    ofCamCoor.setup(_Cam_e, _axis_x, _axis_y, _axis_z, _CamBox_low, _CamBox_high);
    ofScene.setup(model_cen, ofModelCoor, ofArcCoor, ofCamCoor, light);
    cout << "ofModelCoor e :"; _Model_e.info();
    cout << "ofArcCoor e :"; _Arc_e.info();
    cout << "ofCamCoor e :"; _Cam_e.info();
    cout << "ofCamCoor _CamBox_low :"; _CamBox_low.info();
    cout << "ofCamCoor _CamBox_high :"; _CamBox_high.info();
    _ofGlobalToModelMat.makeTFtoLocalmat(ofModelCoor.u_basis, ofModelCoor.v_basis, ofModelCoor.w_basis, ofModelCoor.origin);
    _ofGlobalToModelMat.matprint("_ofGlobalToModelMat:");
    for (int v = 0; v < OBJData.vectices.size(); v++) {
        f3Vector Vertice_in_ModelCoor;
        Vertice_in_ModelCoor = _ofGlobalToModelMat * OBJData.vectices[v];
        OBJData.model_vectices.push_back(Vertice_in_ModelCoor);
    }
    _ofModelToGlobalMat.makeTFtoWorldmat(ofModelCoor.u_basis, ofModelCoor.v_basis, ofModelCoor.w_basis, ofModelCoor.origin);
    _ofModelToGlobalMat.matprint("_ofModelToGlobalMat");
    _ofGlobalToCamMat.makeTFtoLocalmat(ofCamCoor.u_basis, ofCamCoor.v_basis , ofCamCoor.w_basis , ofCamCoor.origin);
   // _ofGlobalToCamMat.matprint("_ofGlobalToCamMat");
    _ofCameraToCanonical_Morth.makeOrthProjectionmat(_CamBox_low, _CamBox_high);
    _ofCameraToCanonical_Mper.makePerProjectionmat(_CamBox_low, _CamBox_high);
    _ofCameraToCanonical_Mper.matprint("_ofCameraToCanonical_Mper:");
    //_ofCameraToCanonical_Morth.matprint("_ofCameraToCanonical_Morth");
    _ofCanonicaltoScreen_Mvp.makeViewPortmat(w,h);
    //_ofCanonicaltoScreen_Mvp.matprint("_ofCanonicaltoScreen_Mvp");
    matf4x4 ModelToScreenMat;
    //ModelToScreenMat = _ofCanonicaltoScreen_Mvp * _ofCameraToCanonical_Morth * _ofGlobalToCamMat * _ofModelToGlobalMat;
    //ModelToScreenMat.matprint("ModelToScreenMat");
}
//Here is the arcball transformation calculation
/*
input: pixel pos of start pos(x,y) and finished pos(xp,yp)
output: the transformation matrix of arcball
*/
matf4x4 ofApp::ArcballMat(float x, float y, float xp, float yp) {
    //cout << "x,y,xp,yp" << x << y << xp << yp << endl;
    //Generalize two points as the unit circle
    f2Vector p(2 * x / w - 1, -2 * (y / h) + 1);//the start point
    f2Vector p_p(2 * xp / w - 1, -2 * (yp / h) + 1);// the finish point
    //cout << "p=" << p.u << " ," << p.v << endl;
    //cout << "p_p=" << p_p.u << " ," << p_p.v << endl;
    f3Vector ps;//the start point in the 3D arcball
    double root_p = p.u * p.u + p.v * p.v;
    if (root_p > 1) {
        ps = f3Vector(p.u, p.v, 0);
    }
    else {
        ps = f3Vector(p.u, p.v, sqrt(1 - root_p));
    }
    f3Vector psp;//the finish point in the 3D arcball
    double root_p_p = p_p.u * p_p.u + p_p.v * p_p.v;
    if (root_p_p > 1) {
        psp = f3Vector(p_p.u, p_p.v, 0);
    }
    else {
        psp = f3Vector(p_p.u, p_p.v, sqrt(1 - root_p_p));
    }
    //cout << "ps->";
    //ps.info();
    //cout << "psp->";
    //psp.info();
    f3Vector u = ps.crossProduct(psp);//the rotation vector
    float theta = std::acos(ps * psp / (psp.norm() * ps.norm()));
    //cout << "theta=" << theta <<" ,u="<< endl;
    u.normalize();
    //u.info();
    matf4x4 ArcResult;
    ArcResult.makeArcballmat(theta, u);
    return ArcResult;
}
//require: pos of light, pos of hit, pos of eye(0,0,0),the color of the vertices(mapping will use this?)
f3Vector ofApp::GouraudShading(f3Vector v,f3Vector normVec3/*have to be normalized*/, f3Vector vcolor) {
    f3Vector light = ofScene.Lights;
    f3Vector lightray = light - v;
    lightray.normalize();
    f3Vector eyeray = v * (-1);//eye at the origin
    f3Vector ref = normVec3 * (2 * (lightray * normVec3)) - lightray;
    ref.normalize();
    eyeray.normalize();
    double PhongMAX0 = std::max(float(0.0), eyeray * ref);
    PhongMAX0 = (float)pow(PhongMAX0, _PhongExp);
    double LabertainMAX0 = std::max(float(0.0), lightray * normVec3);
    vcolor= f3Vector( vcolor.x * LabertainMAX0 * _LightTensity,  vcolor.y * LabertainMAX0 * _LightTensity, vcolor.z * LabertainMAX0 * _LightTensity);
    //cout << "Lambertian:"; vcolor.info();
    vcolor = vcolor+ vcolor * (PhongMAX0)*_LightTensity;// This is for Phong shading
   // cout << "Phong:"; vcolor.info();

    return vcolor;
}
/*
Use this function to get the spherical uv coordination from vertices
*/
f2Vector GetUVCoor_Spherical(f3Vector v) {
    f2Vector UV;
    UV.u = 0.5 + atan2(v.x, v.z)/ 6.2831852;
    UV.v = acos(v.y / v.norm())/3.1415926;
    //printf("calculate UV=(%f,%f)\n", UV.u, UV.v);
    return UV;
}
//Here is vertices shader. In this shader, I will process all the vertices, transform them into the screen space
void ofApp::VerticesShader() {
    matf4x4 ModelToScreenMat,ModelToCameraMat;
    f3Vector RasVertices/*for screen space*/, CamVertices/*for camera space*/;
    //_p_AcrTFmat.matprint("_p_AcrTFmat in vertices shader:");
    if (_ProjectionTYPE == 0) {
        ModelToScreenMat = _ofCanonicaltoScreen_Mvp * _ofCameraToCanonical_Morth * _ofGlobalToCamMat * _ofModelToGlobalMat * _c_AcrTFmat * _p_AcrTFmat;
    }
    else if (_ProjectionTYPE == 1) {
        ModelToScreenMat = _ofCanonicaltoScreen_Mvp * _ofCameraToCanonical_Mper * _ofGlobalToCamMat * _ofModelToGlobalMat * _c_AcrTFmat * _p_AcrTFmat;//for perspective
    }
    ModelToCameraMat = _ofGlobalToCamMat * _ofModelToGlobalMat * _c_AcrTFmat * _p_AcrTFmat;
    ofScene.Lights = _ofGlobalToCamMat *ofScene.Lights;//convert the light in camera space
    double start = omp_get_wtime();
    //#pragma omp parallel for num_threads(10)  It did not work!!!!
    for (int v = 0; v < OBJData.model_vectices.size(); v++) {
        RasVertices = ModelToScreenMat * OBJData.model_vectices[v];
        RasterData.vectices.push_back(RasVertices);
        CamVertices= ModelToCameraMat* OBJData.model_vectices[v];
        RasterData.cam_vectices.push_back(CamVertices);
        if (_Texture == 1) {
            f2Vector UV = GetUVCoor_Spherical(OBJData.model_vectices[v]);
            RasterData.textures[v] = UV;//clear after fragment
            //RasterData.normals_vertices[v] = f3Vector(1,1,1);//
        }

    }
    RasterData.shapes = OBJData.shapes;
    if (OBJData.normals.size() == 0) {//if this model do not contain normals
        //calculate the normals of each faces
        for (int f = 0; f < OBJData.shapes[0].num_faces; f++) {
            FACES T_face;
            T_face = OBJData.shapes[0].faces[f];
            f3Vector v0 = ModelToCameraMat * OBJData.model_vectices[T_face.indices[0].x];//The first vertices || In camera space
            f3Vector v1 = ModelToCameraMat * OBJData.model_vectices[T_face.indices[1].x];//second
            f3Vector v2 = ModelToCameraMat * OBJData.model_vectices[T_face.indices[2].x];//third
            f3Vector v01 = v1 - v0;
            f3Vector v02 = v2 - v0;
            f3Vector N = v01.crossProduct(v02);
            N.normalize();
            RasterData.normals.push_back(N);
            N = N*(-1) ;   // Base on my test, I don't need the inverse
            
            //set the vertices normals as same as the faces
            RasterData.normals_vertices[T_face.indices[0].x] = N+ RasterData.normals_vertices[T_face.indices[0].x];
            RasterData.normals_vertices_count[T_face.indices[0].x] += 1;
            RasterData.normals_vertices[T_face.indices[1].x] = N+ RasterData.normals_vertices[T_face.indices[1].x];
            RasterData.normals_vertices_count[T_face.indices[1].x] += 1;
            RasterData.normals_vertices[T_face.indices[2].x] = N+ RasterData.normals_vertices[T_face.indices[2].x];
            RasterData.normals_vertices_count[T_face.indices[2].x] += 1;
            if (_Texture == 0) {
                //calculate the illumination 
                f3Vector vcolor;
                vcolor = GouraudShading(v0, N, _TestColor);
                RasterData.colors[T_face.indices[0].x] = vcolor + RasterData.colors[T_face.indices[0].x];
                RasterData.colors_count[T_face.indices[0].x] += 1;

                vcolor = GouraudShading(v1, N, _TestColor);
                RasterData.colors[T_face.indices[1].x] = vcolor + RasterData.colors[T_face.indices[1].x];
                RasterData.colors_count[T_face.indices[1].x] += 1;

                vcolor = GouraudShading(v2, N, _TestColor);
                RasterData.colors[T_face.indices[2].x] = vcolor + RasterData.colors[T_face.indices[2].x];
                RasterData.colors_count[T_face.indices[2].x] += 1;
            }
            else if (_Texture==1) {
                //calculate the illumination 
                f3Vector vcolor;
                f3Vector tcolor;// color from texture
                ofFloatColor tofFloatcolor;
                f2Vector tUV;

                tUV = RasterData.textures[T_face.indices[0].x];//UV for v0
                //printf("tUV[%d]:(%f,%f)\n", T_face.indices[0].x, tUV.u, tUV.v);
                tofFloatcolor = _Mypixels.getColor(round(tUV.u * _Mypixels.getWidth()), round(tUV.v * _Mypixels.getHeight()));
                tcolor = f3Vector(tofFloatcolor.r, tofFloatcolor.g, tofFloatcolor.b);
                vcolor = GouraudShading(v0, N, tcolor);
                RasterData.colors[T_face.indices[0].x] = vcolor + RasterData.colors[T_face.indices[0].x];
                RasterData.colors_count[T_face.indices[0].x] += 1;

                tUV = RasterData.textures[T_face.indices[1].x];//for v1
                //printf("tUV[%d]:(%f,%f)\n", T_face.indices[1].x, tUV.u, tUV.v);
                tofFloatcolor = _Mypixels.getColor(round(tUV.u * _Mypixels.getWidth()), round(tUV.v * _Mypixels.getHeight()));
                tcolor = f3Vector(tofFloatcolor.r, tofFloatcolor.g, tofFloatcolor.b);
                vcolor = GouraudShading(v1, N, tcolor);
                RasterData.colors[T_face.indices[1].x] = vcolor + RasterData.colors[T_face.indices[1].x];
                RasterData.colors_count[T_face.indices[1].x] += 1;

                tUV = RasterData.textures[T_face.indices[2].x];//for v2
                //printf("tUV[%d]:(%f,%f)\n", T_face.indices[2].x, tUV.u, tUV.v);
                tofFloatcolor = _Mypixels.getColor(round(tUV.u * _Mypixels.getWidth()), round(tUV.v * _Mypixels.getHeight()));
                tcolor = f3Vector(tofFloatcolor.r, tofFloatcolor.g, tofFloatcolor.b);
                vcolor = GouraudShading(v2, N, tcolor);
                RasterData.colors[T_face.indices[2].x] = vcolor + RasterData.colors[T_face.indices[2].x];
                RasterData.colors_count[T_face.indices[2].x] += 1;
            }

        }
        //avergae the vertices normal;
        for (int _vi = 0; _vi < RasterData.normals_vertices.size(); _vi++) {
            int VerticesCount = RasterData.normals_vertices_count[_vi];
            int ColorCount = RasterData.colors_count[_vi];
            //cout << "VerticesCount: " << VerticesCount << "ColorCount£º" << ColorCount << endl;
            //cout << "origin:RasterData.colors[_vi]->"; RasterData.colors[_vi].info();
            if (VerticesCount > 0) {
                RasterData.normals_vertices[_vi] = RasterData.normals_vertices[_vi] *(1.0 / VerticesCount);
                RasterData.normals_vertices_count[_vi] = 0;//reset the counter
                RasterData.colors[_vi] = RasterData.colors[_vi] * (1.0 / ColorCount);
                //cout << "average:RasterData.colors[_vi]->"  ; RasterData.colors[_vi].info();
                RasterData.colors_count[_vi] = 0;
            }
            
        }
         
    }
    else if (OBJData.normals.size() > 0) {
        //The model has normals
    }
    else {
        cout << "VerticesShader: normals loading error!" << endl;
    }
    
    double finish = omp_get_wtime();
    //cout << "Vertices shader " << finish - start << " s ." << endl;
}
/*
Phong shading similar to Gauraud shading but for each fragment
input the same as Gauraud shading 
*/
f3Vector ofApp::PhongShading(f3Vector v, f3Vector normVec3/*have to be normalized*/, f3Vector vcolor) {
    f3Vector light = ofScene.Lights;
    f3Vector lightray = light - v;
    lightray.normalize();
    f3Vector eyeray = v * (-1);//eye at the origin
    f3Vector ref = normVec3 * (2 * (lightray * normVec3)) - lightray;
    ref.normalize();
    eyeray.normalize();
    double PhongMAX0 = std::max(float(0.0), eyeray * ref);
    PhongMAX0 = (float)pow(PhongMAX0, _PhongExp);
    double LabertainMAX0 = std::max(float(0.0), lightray * normVec3);
    vcolor = f3Vector(vcolor.x * LabertainMAX0 * _LightTensity, vcolor.y * LabertainMAX0 * _LightTensity, vcolor.z * LabertainMAX0 * _LightTensity);
    //cout << "Lambertian:"; vcolor.info();
    vcolor = vcolor + vcolor * (PhongMAX0)*_LightTensity;// This is for Phong shading
    // cout << "Phong:"; vcolor.info();
    return vcolor;
}
std::vector<Fragment> ofApp::LineRas(f3Vector v1, f3Vector v2,f3Vector aV/*this is for calculate the depth info*/) {
    f3Vector leftP, righrP;
    if (v1.x <= v2.x) {
        leftP = v1;
        righrP = v2;
    }
    else {
        leftP = v2;
        righrP = v1;
    }
    std::vector<Fragment> FragmentVec;
    float m;
    float y01 = leftP.y - righrP.y;
    float x10 = righrP.x - leftP.x;
    float C = leftP.x * righrP.y - righrP.x * leftP.y;//x0y1-x1y0
    if (abs(righrP.x - leftP.x) >= 0.001) {
        m = (righrP.y - leftP.y) / (righrP.x - leftP.x);
    }
    else {
        if ((righrP.x - leftP.x) >= 0 && (righrP.x - leftP.x) < 0.001) {
            m = (righrP.y - leftP.y) / (0.001);
        }
        else {
            m = (righrP.y - leftP.y) / (-0.001);
        }
    }
    if (y01 <= 0.001&& y01>0) {
        y01 = 0.001;
    }
    else if (y01 >= -0.001 && y01 < 0) {
        y01 = -0.001;
    }
    if (x10 <= 0.001 && x10 > 0) {
        x10 = 0.001;
    }
    else if (x10 >= -0.001 && x10 < 0) {
        x10 = -0.001;
    }
    //printf("m=%f,x10=%f,y01=%f\n", m, x10, y01);
    Fragment _fragments;
    _fragments.type = 0;
    _fragments.colors = _TestColor;
    //cout << "_fragments created" << endl;
    if (m > 0 && m < 1) {
        int _y = floor(leftP.y);
        for (int _x = floor(leftP.x); _x < ceil(righrP.x); _x++) {
            float A = alphaPara(v1, v2, aV, _x, _y);
            float B = betaPara(v1, v2, aV, _x, _y);
            float R = gamaPara(v1, v2, aV, _x, _y);
            _fragments.pixel = Int2Vector(_x, _y);
            _fragments.depth = (v1 * A + v2 * B + aV * R).z;
            //float ratio = (_x - leftP.x) / x10;
            //_fragments.depth = ratio * leftP.z + (1 - ratio) * righrP.z;
            FragmentVec.push_back(_fragments);
            float FnValue = (_x + 1) * y01 + x10 * (_y+0.5) + C;
            if (FnValue < 0) { _y = _y + 1; }
        }
    }
    else if (m >= 1) {
        int _y = floor(leftP.y);
        int _x = floor(leftP.x);
        for (_y;_y<ceil(righrP.y);_y++) {
            float A = alphaPara(v1, v2, aV, _x, _y);
            float B = betaPara(v1, v2, aV, _x, _y);
            float R = gamaPara(v1, v2, aV, _x, _y);
            _fragments.pixel = Int2Vector(_x, _y);
            _fragments.depth = (v1 * A + v2 * B + aV * R).z;
            _fragments.pixel = Int2Vector(_x, _y);
            //float ratio = (_x - leftP.x) / x10;
            //_fragments.depth = ratio * leftP.z + (1 - ratio) * righrP.z;
            FragmentVec.push_back(_fragments);
            float FnValue = (_x + 0.5) * y01 + x10 * (_y + 1) + C;
            if (FnValue > 0) { _x = _x + 1; }
        }
    }
    else if (m <= 0 && m > -1) {
        int _y = floor(leftP.y);
        int _x = floor(leftP.x);
        for (_x; _x < ceil(righrP.x); _x++) {
            float A = alphaPara(v1, v2, aV, _x, _y);
            float B = betaPara(v1, v2, aV, _x, _y);
            float R = gamaPara(v1, v2, aV, _x, _y);
            _fragments.pixel = Int2Vector(_x, _y);
            _fragments.depth = (v1 * A + v2 * B + aV * R).z;
            _fragments.pixel = Int2Vector(_x, _y);
            //float ratio = (_x - leftP.x) / x10;
            //_fragments.depth = ratio * leftP.z + (1 - ratio) * righrP.z;
            FragmentVec.push_back(_fragments);
            float FnValue = (_x + 1) * y01 + x10 * (_y -0.5) + C;
            if (FnValue > 0) { _y = _y-1; }
        }
    }
    else if (m<=-1) {
        int _y = floor(leftP.y);
        int _x = floor(leftP.x);
        for (_y; _y > floor(righrP.y); _y--) {
            float A = alphaPara(v1, v2, aV, _x, _y);
            float B = betaPara(v1, v2, aV, _x, _y);
            float R = gamaPara(v1, v2, aV, _x, _y);
            _fragments.pixel = Int2Vector(_x, _y);
            _fragments.depth = (v1 * A + v2 * B + aV * R).z;
            _fragments.pixel = Int2Vector(_x, _y);
            //float ratio = (_x - leftP.x) / x10;
            //_fragments.depth = ratio * leftP.z + (1 - ratio) * righrP.z;
            FragmentVec.push_back(_fragments);
            float FnValue = (_x + 0.5) * y01 + x10 * (_y -1) + C;
            if (FnValue < 0) { _x = _x + 1; }
        }
    }
    return FragmentVec;
}
void ofApp::FragmentShader() {
    //cout << "RasterData.shapes[0].num_faces:" << RasterData.shapes[0].num_faces << endl;
    //cout << "RasterData.shapes[0].num_faces:" << RasterData.shapes[0].num_faces << endl;
    for (int f = 0; f < RasterData.shapes[0].num_faces; f++) {
        FACES T_face;
        T_face = RasterData.shapes[0].faces[f];
        f3Vector v0 = RasterData.vectices[T_face.indices[0].x];//The first vertices 
        f3Vector v1 = RasterData.vectices[T_face.indices[1].x];//second
        f3Vector v2 = RasterData.vectices[T_face.indices[2].x];//third

        f3Vector vn0 = RasterData.normals_vertices[T_face.indices[0].x];//The first vertices 
        f3Vector vn1 = RasterData.normals_vertices[T_face.indices[1].x];//second
        f3Vector vn2 = RasterData.normals_vertices[T_face.indices[2].x];//third

        f3Vector c0, c1, c2;
        if (_ShadingTYPE == NORMAL_SHADING) {
             c0 = RasterData.normals_vertices[T_face.indices[0].x];
             c1 = RasterData.normals_vertices[T_face.indices[1].x];
             c2 = RasterData.normals_vertices[T_face.indices[2].x];
        }
        else if (_ShadingTYPE == GAURAUD_SHADING || _ShadingTYPE == PHONG_SHADING) {
            //calculate the real color
             c0 = RasterData.colors[T_face.indices[0].x];
             c1 = RasterData.colors[T_face.indices[1].x];
             c2 = RasterData.colors[T_face.indices[2].x];
        }
        /*

        */
        if (_ShadingTYPE == WIREFRAME) {
            std::vector<Fragment> tFragmentVec = LineRas(v0,v1,v2);
            for (int _f = 0; _f < tFragmentVec.size(); _f++) {
                FragData.fragments.push_back(tFragmentVec[_f]);
            }
            tFragmentVec = LineRas(v1, v2,v0);
            for (int _f = 0; _f < tFragmentVec.size(); _f++) {
                FragData.fragments.push_back(tFragmentVec[_f]);
            }
            tFragmentVec = LineRas(v2, v0,v1);
            for (int _f = 0; _f < tFragmentVec.size(); _f++) {
                FragData.fragments.push_back(tFragmentVec[_f]);
            }
            int x_min = min_x(v0.x, v1.x, v2.x);
            int x_max = max_x(v0.x, v1.x, v2.x);
            int y_min = min_x(v0.y, v1.y, v2.y);
            int y_max = max_y(v0.y, v1.y, v2.y);
            for (int _x = x_min; _x < x_max; _x++) {
                for (int _y = y_min; _y < y_max; _y++) {
                    float A = alphaPara(v0, v1, v2, _x, _y);
                    float B = betaPara(v0, v1, v2, _x, _y);
                    float R = gamaPara(v0, v1, v2, _x, _y);
                    if (A > 0 && B > 0 && R > 0) {
                        f3Vector FragVerPos = v0 * A + v1 * B + v2 * R;//according barycentric coordination interpolation, to get the z-depth
                        f3Vector FragVerNor = vn0 * A + vn1 * B + vn2 * R;//according barycentric coordination interpolation, to get the z-depth
                        FragVerNor.normalize();
                        f3Vector FragPos = f3Vector(_x, _y, FragVerPos.z);
                        f3Vector fragcolor;
                        if (_ShadingTYPE == NORMAL_SHADING || _ShadingTYPE == GAURAUD_SHADING) {
                            fragcolor = c0 * A + c1 * B + c2 * R;
                        }
                        else if (_ShadingTYPE == PHONG_SHADING) {
                            //calculate the real color
                            if (_Texture == 0) {
                                fragcolor = PhongShading(FragVerPos, FragVerNor/*have to be normalized*/, _TestColor);
                            }
                            else if (_Texture == 1) {
                                f3Vector  textureColor = c0 * A + c1 * B + c2 * R;
                                fragcolor = PhongShading(FragVerPos, FragVerNor/*have to be normalized*/, textureColor);
                            }

                        }
                        fragcolor = fragcolor + _ambient;
                        ColorSpace(fragcolor);
                        Fragment _fragments;
                        _fragments.type = 1;
                        _fragments.colors = fragcolor;
                        _fragments.pixel = Int2Vector(_x, _y);
                        _fragments.depth = FragVerPos.z;
                        FragData.fragments.push_back(_fragments);
                    }
                }
            }
        }
        else {
            int x_min = min_x(v0.x, v1.x, v2.x);
            int x_max = max_x(v0.x, v1.x, v2.x);
            int y_min = min_x(v0.y, v1.y, v2.y);
            int y_max = max_y(v0.y, v1.y, v2.y);
            for (int _x = x_min; _x < x_max; _x++) {
                for (int _y = y_min; _y < y_max; _y++) {
                    float A = alphaPara(v0, v1, v2, _x, _y);
                    float B = betaPara(v0, v1, v2, _x, _y);
                    float R = gamaPara(v0, v1, v2, _x, _y);
                    if (A > 0 && B > 0 && R > 0) {
                        f3Vector FragVerPos = v0 * A + v1 * B + v2 * R;//according barycentric coordination interpolation, to get the z-depth
                        f3Vector FragVerNor = vn0 * A + vn1 * B + vn2 * R;//according barycentric coordination interpolation, to get the z-depth
                        FragVerNor.normalize();
                        f3Vector FragPos = f3Vector(_x, _y, FragVerPos.z);
                        f3Vector fragcolor;
                        if (_ShadingTYPE == NORMAL_SHADING || _ShadingTYPE == GAURAUD_SHADING) {
                            fragcolor = c0 * A + c1 * B + c2 * R;
                        }
                        else if (_ShadingTYPE == PHONG_SHADING) {
                            //calculate the real color
                            if (_Texture == 0) {
                                fragcolor = PhongShading(FragVerPos, FragVerNor/*have to be normalized*/, _TestColor);
                            }
                            else if (_Texture == 1) {
                                f3Vector  textureColor = c0 * A + c1 * B + c2 * R;
                                fragcolor = PhongShading(FragVerPos, FragVerNor/*have to be normalized*/, textureColor);
                            }

                        }
                        fragcolor = fragcolor + _ambient;
                        ColorSpace(fragcolor);
                        Fragment _fragments;
                        _fragments.type = 1;
                        _fragments.colors = fragcolor;
                        _fragments.pixel = Int2Vector(_x, _y);
                        _fragments.depth = FragVerPos.z;
                        FragData.fragments.push_back(_fragments);
                    }
                }
            }
        }

    }
    RasterData.colors= std::vector<f3Vector >(RasterData.colors.size(), f3Vector(0, 0, 0));
    RasterData.vectices.clear();
}

void ofApp::FragmentProcess() {
    
    //cout << "FragData.fragments.size():" << FragData.fragments.size() << endl;
    colorPixels.clear();//clear first!
    colorPixels.allocate(w, h, OF_PIXELS_RGBA);//reset the screen
    if (_ShadingTYPE == WIREFRAME) {
        for (int _f = 0; _f < FragData.fragments.size(); _f++) {
            Fragment _tF = FragData.fragments[_f];
            if (_tF.depth > FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y] && _tF.type == 1) {
                FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y] = _tF.depth;//fill out the z-buffer first
            }
        }
        for (int _f = 0; _f < FragData.fragments.size(); _f++) {
            Fragment _tF = FragData.fragments[_f];
            if (_tF.depth >= FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y] && _tF.type == 0) {
                colorPixels.setColor(_tF.pixel.x, _tF.pixel.y, ofColor((_tF.colors.x) * 255, (_tF.colors.y) * 255, (_tF.colors.z) * 255));
                FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y] = _tF.depth;
            }
        }
    }
    else {
        for (int _f = 0; _f < FragData.fragments.size(); _f++) {
            Fragment _tF = FragData.fragments[_f];
            if (_tF.depth > FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y]) {
                if (_ShadingTYPE == NORMAL_SHADING) {
                    f3Vector Poscolor = _tF.colors * 0.5 + f3Vector(0.5, 0.5, 0.5);//keep it positive
                    //_tF.colors.info();
                    //f3Vector Poscolor = _tF.colors;
                    //_tF.colors.info();
                    //colorPixels.setColor(_tF.pixel.x, _tF.pixel.y, ofColor(-1 * 255, 0.2 * 255, -0.3 * 255));
                    colorPixels.setColor(_tF.pixel.x, _tF.pixel.y, ofColor((Poscolor.x) * 255, (Poscolor.y) * 255, (Poscolor.z) * 255));
                    FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y] = _tF.depth;
                }
                else if (_ShadingTYPE == PHONG_SHADING || _ShadingTYPE == GAURAUD_SHADING) {
                    colorPixels.setColor(_tF.pixel.x, _tF.pixel.y, ofColor((_tF.colors.x) * 255, (_tF.colors.y) * 255, (_tF.colors.z) * 255));
                    FragData.z_Buffer[_tF.pixel.x][_tF.pixel.y] = _tF.depth;
                }
            }

        }
    }

    texColor.loadData(colorPixels);
    FragData.z_Buffer=std::vector<std::vector<float>>(w, std::vector<float>(h, Z_MIN));
    FragData.fragments.clear();

    /*
    colorPixels.clear();//clear first!
    colorPixels.allocate(w, h, OF_PIXELS_RGBA);//reset the screen
    cout << "RasterData.shapes[0].num_faces:" << RasterData.shapes[0].num_faces << endl;
    for (int f = 0; f < RasterData.shapes[0].num_faces; f++) {
        FACES T_face;
        T_face = RasterData.shapes[0].faces[f];
        f3Vector v0 = RasterData.vectices[T_face.indices[0].x];//The first vertices 
        f3Vector v1 = RasterData.vectices[T_face.indices[1].x];//second
        f3Vector v2 = RasterData.vectices[T_face.indices[2].x];//third
       //cout << "normal RasterData.normals[f],f="<<f<<endl ;
        //RasterData.normals[f].info();
        f3Vector Testcolor = RasterData.normals[f] * 0.5 + f3Vector(0.5, 0.5, 0.5);//keep it positive
        colorPixels.setColor(v0.x, v0.y, ofColor((Testcolor.x) * 255, (Testcolor.y) * 255, (Testcolor.z) * 255));
        colorPixels.setColor(v1.x, v1.y, ofColor((Testcolor.x) * 255, (Testcolor.y) * 255, (Testcolor.z) * 255));
        colorPixels.setColor(v2.x, v2.y, ofColor((Testcolor.x) * 255, (Testcolor.y) * 255, (Testcolor.z) * 255));
    }
    texColor.loadData(colorPixels);
    RasterData.vectices.clear();
    */
}
