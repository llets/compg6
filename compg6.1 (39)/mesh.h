#ifndef MESH_H
#define	MESH_H

#include <map>
#include <vector>
#include <assert.h>
#include <assimp/Importer.hpp>  
#include <assimp/scene.h>       
#include <assimp/postprocess.h> 
#include <glm/glm.hpp>

#include "texture.h"

using namespace std;

// Stores an edge by its vertices and force an order between them
struct Edge
{
    Edge(unsigned int _a, unsigned int _b)
    {
        assert(_a != _b);
        
        if (_a < _b)
        {
            a = _a;
            b = _b;                   
        }
        else
        {
            a = _b;
            b = _a;
        }
    }
    
    unsigned int a;
    unsigned int b;
};

struct Neighbors
{
    unsigned int n1;
    unsigned int n2;
    
    Neighbors()
    {
        n1 = n2 = (unsigned int)-1;
    }
    
    void AddNeigbor(unsigned int n)
    {
        if (n1 == -1) {
            n1 = n;
        }
        else if (n2 == -1) {
            n2 = n;
        }
        else {
            assert(0);
        }
    }
    
    unsigned int GetOther(unsigned int me) const
    {
        if (n1 == me) {
            return n2;
        }
        else if (n2 == me) {
            return n1;
        }
        else {
            assert(0);
        }

        return 0;
    }
};

struct CompareEdges
{
    bool operator()(const Edge& Edge1, const Edge& Edge2) const
    {
        if (Edge1.a < Edge2.a) {
            return true;
        }
        else if (Edge1.a == Edge2.a) {
            return (Edge1.b < Edge2.b);
        }        
        else {
            return false;
        }            
    }
};

class Mesh
{
public:
    Mesh();
    ~Mesh();
    bool LoadMesh(const string& Filename, bool WithAdjacencies);
    void Render();
    unsigned int NumBones() const
    {
        return m_NumBones;
    }
    void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);
    
private:
    #define NUM_BONES_PER_VEREX 4

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;        

        BoneInfo()
        {
            //BoneOffset.SetZero();
            //FinalTransformation.SetZero();            
        }
    };
    
    struct VertexBoneData
    {        
        unsigned int IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            memset(IDs, 0, sizeof(IDs));
            memset(Weights, 0, sizeof(Weights));
        }
        
        void AddBoneData(unsigned int BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
    bool InitFromScene(const aiScene* pScene, const string& Filename);
    void FindAdjacencies(const aiMesh* paiMesh, vector<unsigned int>& Indices);
    void InitMesh(unsigned int MeshIndex,
                  const aiMesh* paiMesh,
                  vector<glm::vec3>& Positions,
                  vector<glm::vec3>& Normals,
                  vector<glm::vec2>& TexCoords,
                  vector<VertexBoneData>& Bones,
                  vector<unsigned int>& Indices);
    void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
    bool InitMaterials(const aiScene* pScene, const string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    vector<MeshEntry> m_Entries;
    vector<Texture*> m_Textures;
    map<string,unsigned int> m_BoneMapping; // maps a bone name to its index
    unsigned int m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    glm::mat4 m_GlobalInverseTransform;
    std::map<Edge, Neighbors, CompareEdges> m_indexMap;
    bool m_withAdjacencies;
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
};
#endif