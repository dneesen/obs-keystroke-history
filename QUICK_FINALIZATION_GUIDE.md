# Quick Finalization Reference Card

**Version:** 1.2.1  
**Purpose:** Fast reference for release finalization

---

## 🚀 Option 1: Automated Script (Recommended)

```powershell
# Navigate to project
cd "d:\OBS Plugins\Keystroke History"

# Run finalization script
.\finalize-release.ps1

# With auto-commit (advanced)
.\finalize-release.ps1 -AutoCommit

# Cleanup only
.\finalize-release.ps1 -CleanOnly
```

**What it does:**
- ✓ Cleans old docs
- ✓ Rebuilds from scratch
- ✓ Creates ZIP and EXE
- ✓ Verifies packages
- ✓ Optional: Git commit & tag

---

## 🔧 Option 2: Manual Steps

### Quick Build & Package
```powershell
cd "d:\OBS Plugins\Keystroke History"
Remove-Item build -Recurse -Force
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\create-package.ps1
.\build-installer.ps1
```

### Quick Git Commit
```powershell
git add -A
git commit -m "Release v1.2.1 - Unicode arrow keys fix"
git tag -a v1.2.1 -m "Release v1.2.1"
git push origin main
git push origin v1.2.1
```

---

## 📋 Pre-Flight Checklist

- [ ] Arrow keys tested (←, ↑, →, ↓)
- [ ] All fonts working (Arial, Consolas, etc.)
- [ ] Text alignment works (L/C/R)
- [ ] No errors in OBS logs
- [ ] CMakeLists.txt version = 1.2.1
- [ ] installer.iss version = 1.2.1

---

## 📦 Expected Output Files

After finalization, you should have:

```
obs-keystroke-history-v1.2.1-manual-install.zip     (~50 KB)
obs-keystroke-history-installer-v1.2.1.exe          (~1-2 MB)
```

---

## 🗑️ Files to Delete

Remove these old documentation files:
```powershell
Remove-Item "UNICODE_ARROW_KEYS_FIX.md" -Force
Remove-Item "TESTING_v1.2.0.md" -Force
Remove-Item "RELEASE_v1.2.0.md" -Force
Remove-Item "WHATS_NEW_v1.2.0.md" -Force
```

Or use:
```powershell
.\finalize-release.ps1 -CleanOnly
```

---

## 📝 Files to Keep

✓ `README.md` - Main documentation  
✓ `BUILD_GUIDE.md` - Build instructions  
✓ `PACKAGING.md` - Packaging instructions  
✓ `LICENSE` - License file  
✓ `RELEASE_v1.2.1.md` - Current release notes  
✓ `WHATS_NEW_v1.2.1.md` - Current what's new  
✓ `RELEASE_FINALIZATION_CHECKLIST.md` - This checklist

---

## 🔍 Verification Commands

### Check DLL exists
```powershell
Test-Path "build\Release\keystroke-history.dll"
# Should return: True
```

### Check packages exist
```powershell
Test-Path "obs-keystroke-history-v1.2.1-manual-install.zip"
Test-Path "obs-keystroke-history-installer-v1.2.1.exe"
# Both should return: True
```

### Check Git status
```powershell
git status
git log --oneline -5
git tag -l
```

---

## 🌐 GitHub Release Steps

1. Go to: https://github.com/dneesen/obs-keystroke-history/releases/new
2. Tag: `v1.2.1`
3. Title: `v1.2.1 - Unicode Arrow Keys Fix`
4. Description: Copy from `RELEASE_v1.2.1.md`
5. Attach:
   - `obs-keystroke-history-installer-v1.2.1.exe`
   - `obs-keystroke-history-v1.2.1-manual-install.zip`
6. ✓ Set as latest release
7. Publish!

---

## ⚠️ Common Issues

### CMake fails
**Solution:** Ensure Visual Studio 2022 is installed
```powershell
cmake --version  # Should show 3.16+
```

### DLL not created
**Solution:** Check build output for errors
```powershell
cmake --build build --config Release --verbose
```

### Git push rejected
**Solution:** Pull first, then push
```powershell
git pull origin main
git push origin main
git push origin v1.2.1
```

### Installer script fails
**Solution:** Ensure Inno Setup is installed and in PATH

---

## 📞 Emergency Rollback

If something goes wrong:

```powershell
# Revert to last commit
git reset --hard HEAD~1

# Delete tag
git tag -d v1.2.1

# Force push (if already pushed - use with caution!)
# git push origin main --force
# git push origin :refs/tags/v1.2.1
```

---

## ✅ Success Indicators

You're done when:
- ✅ Both distribution files created
- ✅ No build errors
- ✅ Git commit & tag pushed
- ✅ GitHub release published
- ✅ Old docs deleted
- ✅ Version numbers match everywhere

---

## 📖 Full Documentation

For complete details, see:
**`RELEASE_FINALIZATION_CHECKLIST.md`**

---

**Last Updated:** October 13, 2025
