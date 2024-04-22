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
import GameplayKit


class GameScene: SKScene {
    
    override func didMove(to view: SKView) {
        
        if let size = NSApp.windows.first?.frame.size
        {
            self.size = CGSizeMake(size.width, size.height)
            fxCGDisplayNode.shared.position = .init(x: size.width / 2, y: size.height / 2)
        }
        
        addChild(fxCGDisplayNode.shared)
        
        DispatchQueue.global(qos: .userInteractive).async {
            fxCG_g3a()
            exit(0)
        }
    }
    
    
    func touchDown(atPoint pos : CGPoint) {
        
    }
    
    func touchMoved(toPoint pos : CGPoint) {
        
    }
    
    func touchUp(atPoint pos : CGPoint) {
        
    }
    
    override func mouseDown(with event: NSEvent) {
        self.touchDown(atPoint: event.location(in: self))
    }
    
    override func mouseDragged(with event: NSEvent) {
        self.touchMoved(toPoint: event.location(in: self))
    }
    
    override func mouseUp(with event: NSEvent) {
        self.touchUp(atPoint: event.location(in: self))
    }
    
    override func keyDown(with event: NSEvent) {
        switch event.keyCode {
        case 122:
            fxCG_KeyDown(KEY_PRGM_F1)
        case 120:
            fxCG_KeyDown(KEY_PRGM_F2)
        case 99:
            fxCG_KeyDown(KEY_PRGM_F3)
        case 118:
            fxCG_KeyDown(KEY_PRGM_F4)
        case 96:
            fxCG_KeyDown(KEY_PRGM_F5)
        case 97:
            fxCG_KeyDown(KEY_PRGM_F6)
        case 51:
            fxCG_KeyDown(44)
        case 53:
            fxCG_KeyDown(KEY_PRGM_EXIT)
        case 36:
            fxCG_KeyDown(KEY_PRGM_RETURN)
        case 76:
            fxCG_KeyDown(KEY_PRGM_MENU)
        case 117:
            fxCG_KeyDown(KEY_PRGM_ACON)
        case 29:
            fxCG_KeyDown(KEY_PRGM_0)
        case 18:
            fxCG_KeyDown(KEY_PRGM_1)
        case 19:
            fxCG_KeyDown(KEY_PRGM_2)
        case 20:
            fxCG_KeyDown(KEY_PRGM_3)
        case 21:
            fxCG_KeyDown(KEY_PRGM_4)
        case 23:
            fxCG_KeyDown(KEY_PRGM_5)
        case 22:
            fxCG_KeyDown(KEY_PRGM_6)
        case 24:
            fxCG_KeyDown(42)
        case 27:
            fxCG_KeyDown(32)
        case 26:
            fxCG_KeyDown(KEY_PRGM_7)
        case 28:
            fxCG_KeyDown(KEY_PRGM_8)
        case 25:
            fxCG_KeyDown(KEY_PRGM_9)
        case 124:
            fxCG_KeyDown(KEY_PRGM_RIGHT)
        case 123:
            fxCG_KeyDown(KEY_PRGM_LEFT)
        case 126:
            fxCG_KeyDown(KEY_PRGM_UP)
        case 125:
            fxCG_KeyDown(KEY_PRGM_DOWN)
            
        default:
            print("keyDown: \(event.characters!) keyCode: \(event.keyCode)")
        }
    }

    
    override func keyUp(with event: NSEvent) {
        switch event.keyCode {
        case 122:
            fxCG_KeyUp(KEY_PRGM_F1)
        case 120:
            fxCG_KeyUp(KEY_PRGM_F2)
        case 99:
            fxCG_KeyUp(KEY_PRGM_F3)
        case 118:
            fxCG_KeyUp(KEY_PRGM_F4)
        case 96:
            fxCG_KeyUp(KEY_PRGM_F5)
        case 97:
            fxCG_KeyUp(KEY_PRGM_F6)
        case 51:
            fxCG_KeyUp(44)
        case 53:
            fxCG_KeyUp(KEY_PRGM_EXIT)
        case 76:
            fxCG_KeyUp(KEY_PRGM_MENU)
        case 117:
            fxCG_KeyUp(KEY_PRGM_ACON)
        case 36:
            fxCG_KeyUp(KEY_PRGM_RETURN)
        case 29:
            fxCG_KeyUp(KEY_PRGM_0)
        case 18:
            fxCG_KeyUp(KEY_PRGM_1)
        case 19:
            fxCG_KeyUp(KEY_PRGM_2)
        case 20:
            fxCG_KeyUp(KEY_PRGM_3)
        case 21:
            fxCG_KeyUp(KEY_PRGM_4)
        case 23:
            fxCG_KeyUp(KEY_PRGM_5)
        case 22:
            fxCG_KeyUp(KEY_PRGM_6)
        case 26:
            fxCG_KeyUp(KEY_PRGM_7)
        case 24:
            fxCG_KeyUp(42)
        case 27:
            fxCG_KeyUp(32)
        case 28:
            fxCG_KeyUp(KEY_PRGM_8)
        case 25:
            fxCG_KeyUp(KEY_PRGM_9)
        case 124:
            fxCG_KeyUp(KEY_PRGM_RIGHT)
        case 123:
            fxCG_KeyUp(KEY_PRGM_LEFT)
        case 126:
            fxCG_KeyUp(KEY_PRGM_UP)
        case 125:
            fxCG_KeyUp(KEY_PRGM_DOWN)
            
        default:
            print("keyUp: \(event.characters!) keyCode: \(event.keyCode)")
        }
    }
    
    override func update(_ currentTime: TimeInterval) {
        fxCGDisplayNode.shared.redraw()
    }
}

extension NSApplication {
    static var rootViewController: NSViewController? {
        return NSApplication.shared.windows.first?.contentViewController
    }
}



