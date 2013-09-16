import Tkinter          as tk
import tkColorChooser   as tkcolor
import ttk
import pygame

from zEntity import *
import zGUI

DEFAULT_LIGHT = {
    'type':         'POINT',
    'color':        '1.00,1.00,1.00',
    'attenuation':  '0.05,0.01,0.00',
    'brightness':   '1.0'
}

def make2f(s):
    parts = s.strip().split(',')
    return ftos([float(x) for x in parts])

def ftos(f):
    return ','.join('%0.2f' % x for x in f)
    
class LightPropertyWindow(zGUI.PropertyWindow):
    def __init__(self, light, parent):
        apply(zGUI.PropertyWindow.__init__, (self, parent, light))
        
        self.TypeVar    = tk.StringVar()
        self.ColorVar   = tk.StringVar()

        self.TypeVar .set(light.details['type'].upper())
        self.ColorVar.set(light.details['color'])

        self.Point  = ttk.Radiobutton(self, text='Point',
                                      variable=self.TypeVar, value='POINT')
        self.Spot   = ttk.Radiobutton(self, text='Spotlight',
                                      variable=self.TypeVar, value='SPOT')
        self.Ambient= ttk.Radiobutton(self, text='Ambient',
                                      variable=self.TypeVar, value='AMBIENT')

        self.Color  = tk.Frame(self, bg='#FFFFFF', width=100, height=100)
        self.Color.bind('<ButtonRelease-1>', self._Evt_ChooseColor)

        tk.Label(self, text='All entries that require floating point '
                            'values should be comma-separated.').grid(
                            column=0, row=0, columnspan=2)
        tk.Label(self,   text='Lighting Type: ').grid(row=2, column=0, sticky='e')
        tk.Label(self, text='Color: ').grid(row=4, column=0, sticky='e')

        self.Point  .grid(row=1, column=1, sticky='w')
        self.Spot   .grid(row=2, column=1, sticky='w')
        self.Ambient.grid(row=3, column=1, sticky='w')
        self.Color.grid(row=4, column=1, sticky='w', padx=10)
        
    def Exit(self):
        self.applied.details['type'] = self.TypeVar.get()
        self.applied.details['color'] = self.ColorVar.get()
        color = [min(float(x) * 255, 255) for x in self.ColorVar.get().split(',')]
        surf = pygame.Surface((self.applied.surface.get_width(),
                               self.applied.surface.get_height()))
        surf.fill(tuple(color))
        surf.blit(pygame.image.load('light.png'), (0, 0))
        self.applied.surface = surf
        self.destroy()

    def _Evt_ChooseColor(self, evt=None):
        (rgb, hex) = tkcolor.askcolor('#' + ''.join(['%02x' % int(min(float(x)*255, 255)) \
            for x in self.ColorVar.get().split(',')
        ]), title='Choose a Light Color...', parent=self)
        self.ColorVar.set(','.join(str(x / 255.0) for x in rgb))
        self.Color.config(bg=hex)

class Light(Entity):
    def __init__(self, d=DEFAULT_LIGHT):
        apply(Entity.__init__, (self, ))
        self.details = dict(d)
        self.Load(filename='light.png')
        surf = pygame.Surface((self.surface.get_width(),
                               self.surface.get_height()))
        surf.fill((255, 255, 255))
        surf.blit(self.surface, (0, 0))
        self.surface = surf
