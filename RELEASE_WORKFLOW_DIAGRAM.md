# Release Finalization Workflow

```
╔════════════════════════════════════════════════════════════════╗
║                   RELEASE FINALIZATION WORKFLOW                 ║
║                          Version 1.2.1                          ║
╚════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────────┐
│  PHASE 1: TESTING & VERIFICATION                                │
└─────────────────────────────────────────────────────────────────┘

    ┌─────────────────┐
    │ Test Arrow Keys │  ←  ↑  →  ↓
    │   (←, ↑, →, ↓)  │
    └────────┬────────┘
             │
             ↓
    ┌─────────────────┐
    │  Test All Fonts │  Arial, Consolas, etc.
    └────────┬────────┘
             │
             ↓
    ┌─────────────────┐
    │ Verify Features │  Alignment, Fading, etc.
    └────────┬────────┘
             │
             ↓
    ┌─────────────────┐
    │  Check OBS Logs │  No errors?
    └────────┬────────┘
             │
             ↓
         ✅ PASS? ──────> Continue
             │
         ❌ FAIL? ──────> Fix Issues & Retest


┌─────────────────────────────────────────────────────────────────┐
│  PHASE 2: VERSION UPDATES                                       │
└─────────────────────────────────────────────────────────────────┘

    ┌──────────────────┐
    │ CMakeLists.txt   │  project(... VERSION 1.2.1)
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  installer.iss   │  #define MyAppVersion "1.2.1"
    └────────┬─────────┘
             │
             ↓
         ✅ Updated


┌─────────────────────────────────────────────────────────────────┐
│  PHASE 3: BUILD & PACKAGE                                       │
└─────────────────────────────────────────────────────────────────┘

    ╔════════════════════════════════════╗
    ║   OPTION A: AUTOMATED (Recommended)║
    ╚════════════════════════════════════╝
    
    ┌──────────────────────────────────┐
    │  .\finalize-release.ps1          │
    │                                  │
    │  • Cleans old docs               │
    │  • Rebuilds from scratch         │
    │  • Creates ZIP                   │
    │  • Creates EXE                   │
    │  • Verifies output               │
    │  • Optional: Git commit          │
    └────────────┬─────────────────────┘
                 │
                 ↓
    ┌──────────────────────────────────┐
    │   ✓ Finalization Complete!       │
    └────────────┬─────────────────────┘
                 │
                 └─────────────────> Continue to Phase 4


    ╔════════════════════════════════════╗
    ║   OPTION B: MANUAL STEPS           ║
    ╚════════════════════════════════════╝
    
    ┌──────────────────┐
    │  Clean Build     │  Remove-Item build -Recurse -Force
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  Configure CMake │  cmake -S . -B build ...
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  Build Release   │  cmake --build build --config Release
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  Create Package  │  .\create-package.ps1
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │ Build Installer  │  .\build-installer.ps1
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │ Verify Outputs   │  ZIP + EXE exist?
    └────────┬─────────┘
             │
             └─────────────────> Continue to Phase 4


┌─────────────────────────────────────────────────────────────────┐
│  PHASE 4: FILE CLEANUP                                          │
└─────────────────────────────────────────────────────────────────┘

    ┌────────────────────────────────────┐
    │  Delete Old Documentation:         │
    │                                    │
    │  ❌ UNICODE_ARROW_KEYS_FIX.md     │
    │  ❌ TESTING_v1.2.0.md             │
    │  ❌ RELEASE_v1.2.0.md             │
    │  ❌ WHATS_NEW_v1.2.0.md           │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  Keep Current Documentation:       │
    │                                    │
    │  ✓ README.md                       │
    │  ✓ BUILD_GUIDE.md                  │
    │  ✓ RELEASE_v1.2.1.md              │
    │  ✓ WHATS_NEW_v1.2.1.md            │
    │  ✓ Finalization docs               │
    └────────────┬───────────────────────┘
                 │
                 ↓
         ✅ Cleanup Complete


┌─────────────────────────────────────────────────────────────────┐
│  PHASE 5: VERSION CONTROL                                       │
└─────────────────────────────────────────────────────────────────┘

    ┌──────────────────┐
    │  git add -A      │  Stage all changes
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  git commit      │  "Release v1.2.1 - Unicode arrow keys fix"
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  git tag         │  v1.2.1
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  git push        │  origin main
    └────────┬─────────┘
             │
             ↓
    ┌──────────────────┐
    │  git push        │  origin v1.2.1
    └────────┬─────────┘
             │
             ↓
         ✅ Pushed to GitHub


┌─────────────────────────────────────────────────────────────────┐
│  PHASE 6: GITHUB RELEASE                                        │
└─────────────────────────────────────────────────────────────────┘

    ┌────────────────────────────────────┐
    │  Go to GitHub Releases             │
    │  github.com/.../releases/new       │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  Select Tag: v1.2.1                │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  Title: v1.2.1 - Unicode Arrow     │
    │         Keys Fix                   │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  Description:                      │
    │  Copy from RELEASE_v1.2.1.md       │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  Attach Files:                     │
    │  • ...installer-v1.2.1.exe         │
    │  • ...v1.2.1-manual-install.zip    │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  ☑ Set as latest release           │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  [Publish Release]                 │
    └────────────┬───────────────────────┘
                 │
                 ↓
         ✅ RELEASE PUBLISHED!


┌─────────────────────────────────────────────────────────────────┐
│  PHASE 7: VERIFICATION                                          │
└─────────────────────────────────────────────────────────────────┘

    ┌────────────────────────────────────┐
    │  ✓ GitHub release is visible       │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  ✓ Both files downloadable         │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  ✓ Version numbers match           │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  ✓ Documentation accurate          │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  ✓ Old docs removed from repo      │
    └────────────┬───────────────────────┘
                 │
                 ↓
    ┌────────────────────────────────────┐
    │  ✓ Tags pushed successfully        │
    └────────────┬───────────────────────┘
                 │
                 ↓
         ✅ ALL VERIFIED!


╔════════════════════════════════════════════════════════════════╗
║                        🎉 SUCCESS! 🎉                          ║
║                                                                ║
║                v1.2.1 Release Complete                         ║
║                                                                ║
║              Next Steps (Optional):                            ║
║              • Announce on forums/social media                 ║
║              • Update documentation sites                      ║
║              • Monitor GitHub issues                           ║
║              • Plan next release                               ║
╚════════════════════════════════════════════════════════════════╝


┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃  TROUBLESHOOTING                                               ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

Problem: Build fails
Solution: Check Visual Studio installed, run: cmake --version

Problem: DLL not created
Solution: Check for compilation errors in build output

Problem: Git push rejected
Solution: git pull origin main, then push again

Problem: Installer fails
Solution: Ensure Inno Setup installed and in PATH

Problem: Wrong version number
Solution: Check CMakeLists.txt and installer.iss match


┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃  QUICK COMMANDS                                                ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

Full Automation:
  .\finalize-release.ps1

Manual Build:
  cmake -S . -B build -G "Visual Studio 17 2022" -A x64
  cmake --build build --config Release
  .\create-package.ps1
  .\build-installer.ps1

Git Commands:
  git add -A
  git commit -m "Release v1.2.1 - Unicode arrow keys fix"
  git tag -a v1.2.1 -m "Release v1.2.1"
  git push origin main
  git push origin v1.2.1

Cleanup:
  .\finalize-release.ps1 -CleanOnly


┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃  DOCUMENTATION REFERENCE                                       ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

Full Details:     RELEASE_FINALIZATION_CHECKLIST.md
Quick Reference:  QUICK_FINALIZATION_GUIDE.md
Summary:          FINALIZATION_DOCUMENTATION_SUMMARY.md
This Workflow:    RELEASE_WORKFLOW_DIAGRAM.md


Last Updated: October 13, 2025
```
