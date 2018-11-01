# -*- mode: python -*-

block_cipher = None


a = Analysis(['test.py'],
             pathex=['D:\\VELA\\GIT Projects\\VELA-CLARA-Controllers\\apps\\llrf_data_transfer_test_pyd\\bin\\Release'],
             binaries=[],
             datas=[],
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name='llrf_daq_test',
          debug=False,
          strip=False,
          upx=True,
          runtime_tmpdir=None,
          console=True )
