/*
 Copyright © 2023 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

import SpriteKit


class fxCGDisplayNode: SKSpriteNode {
    
    static let shared = fxCGDisplayNode()
    
    // MARK: - Private class constants
    private var mutableTexture: SKMutableTexture!
    
    
    
    // MARK: - Init
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }
    
    override init(texture: SKTexture?, color: SKColor, size: CGSize) {
        super.init(texture: texture, color: color, size: size)
    }
    
    convenience init() {
        self.init(color: .clear, size: CGSize(width: 396, height: 224))
        setup()
    }
    
    
    
    // MARK: - Settings...
    private func setup() {
        isUserInteractionEnabled = false
        
        mutableTexture = SKMutableTexture(size: CGSize(width: 396, height: 224))
        texture = mutableTexture
        blendMode = .replace
        zPosition = 1
        
        
        Bdisp_Fill_VRAM(Int32(0xffff), 3)
        DrawFrame(UInt16(0xffff))
        Bdisp_PutDisp_DD()
        
        shader = createShader()
    }
    
    func createShader() -> SKShader? {
        let size = getSceneResolution()
        
        let fxCGShader = SKShader(fileNamed: "fx-CG.fsh")
        fxCGShader.uniforms = [
            SKUniform(name: "u_resolution", vectorFloat3: size),
            SKUniform(name: "u_texture0", texture: mutableTexture),
            SKUniform(name: "u_color_scale", float: 2.0)
        ]
        fxCGShader.attributes = [
            SKAttribute(name: "a_size", type: .vectorFloat2)
        ]
        return fxCGShader
        
        
        func getSceneResolution() -> SIMD3<Float> {
            let width = Float(mutableTexture.size().width)
            let height = Float(mutableTexture.size().height)
            let size = SIMD3<Float>([width, height, 0])
            return size
        }
    }

    
    func redraw() {
        mutableTexture.modifyPixelData { pixelData, lengthInBytes in
            fxCG_DisplayRedraw(pixelData)
        }
    }
    
}
