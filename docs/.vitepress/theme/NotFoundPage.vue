<script setup lang="ts">
import { withBase } from 'vitepress'
</script>

<template>
  <div class="nf-wrap">
    <div class="nf-card">
      <img
        :src="withBase('/icon_128.png')"
        alt="HitPaw icon"
        width="96"
        height="96"
        class="nf-icon"
      />
      <p class="nf-code">404</p>
      <h1 class="nf-title">That page doesn&rsquo;t exist</h1>
      <p class="nf-sub">
        You might have followed an old link — HitPaw moved fast.
        The docs search (<kbd>/</kbd> or <kbd>K</kbd>) usually finds it in a second.
      </p>
      <div class="nf-actions">
        <a class="nf-btn nf-btn--brand" :href="withBase('/')">← Back to Home</a>
        <a class="nf-btn" :href="withBase('/getting-started.html')">Getting Started →</a>
        <a class="nf-btn" :href="withBase('/download.html')">Download →</a>
        <a class="nf-btn" :href="withBase('/export.html')">Export →</a>
      </div>
      <p class="nf-tip">
        <strong>Try the search</strong> — press <kbd>/</kbd> or <kbd>K</kbd> and type.
        Still lost?
        <a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new" target="_blank" rel="noreferrer">Open an issue</a>
        if you think this page should exist — we fix dead links fast.
      </p>
    </div>
  </div>
</template>

<style scoped>
.nf-wrap {
  display: flex;
  justify-content: center;
  padding: 48px 24px 80px;
}
.nf-card {
  max-width: 640px;
  width: 100%;
  text-align: center;
  border: 1px solid var(--hp-line);
  border-radius: var(--hp-r-xl);
  background:
    radial-gradient(32rem 16rem at 50% -20%, rgba(255, 106, 0, .12), transparent 62%),
    var(--hp-surface);
  padding: 44px 28px 36px;
}
.nf-icon {
  display: inline-block;
  opacity: .92;
  filter: drop-shadow(0 8px 20px rgba(255, 106, 0, .22));
}
.nf-code {
  margin: 14px 0 0;
  font-family: var(--vp-font-family-mono);
  font-size: .8rem;
  font-weight: 700;
  letter-spacing: .3em;
  color: var(--hp-orange);
}
.nf-title {
  margin: 8px 0 10px;
  font-size: clamp(1.5rem, 4vw, 1.9rem);
  font-weight: 800;
  letter-spacing: -.025em;
  color: var(--hp-text-hi);
  border: none;
  padding: 0;
}
.nf-title::before {
  content: '';
  display: block;
  width: 44px;
  height: 3px;
  border-radius: var(--hp-r-full);
  margin: 0 auto 14px;
  background: linear-gradient(90deg, var(--hp-orange), var(--hp-orange-soft) 70%, transparent);
}
.nf-sub {
  margin: 0 auto;
  max-width: 460px;
  color: var(--hp-text-mute);
  line-height: 1.7;
  font-size: .96rem;
}
.nf-sub kbd,
.nf-tip kbd {
  display: inline-block;
  padding: 1px 7px;
  border: 1px solid var(--hp-line-2);
  border-bottom-width: 2px;
  border-radius: 6px;
  background: rgba(255, 255, 255, .05);
  color: var(--hp-orange-soft);
  font-family: var(--vp-font-family-mono);
  font-size: .82em;
}
.nf-actions {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  justify-content: center;
  margin: 26px 0 4px;
}
.nf-btn {
  display: inline-flex;
  align-items: center;
  padding: 11px 18px;
  border: 1px solid var(--hp-line-2);
  border-radius: var(--hp-r-full);
  text-decoration: none;
  font-weight: 600;
  font-size: .92rem;
  min-height: 44px;
  background: rgba(255, 255, 255, .04);
  color: #ededf2;
  transition: border-color var(--hp-fast) var(--hp-ease), transform var(--hp-fast) var(--hp-ease), box-shadow var(--hp-fast) var(--hp-ease);
}
.nf-btn:hover {
  border-color: var(--hp-line-brand);
  color: var(--hp-orange-soft);
  transform: translateY(-1px);
  box-shadow: var(--hp-glow);
}
.nf-btn--brand {
  background: linear-gradient(135deg, var(--hp-orange), var(--hp-orange-hi));
  border-color: transparent;
  color: #140700;
  box-shadow: 0 6px 24px rgba(255, 106, 0, .35);
}
.nf-btn--brand:hover {
  color: #140700;
  filter: brightness(1.05);
}
.nf-tip {
  margin: 22px auto 0;
  max-width: 460px;
  color: var(--hp-text-faint);
  font-size: .86rem;
  line-height: 1.7;
  border-top: 1px dashed var(--hp-line);
  padding-top: 18px;
}
.nf-tip a {
  color: var(--hp-orange-soft);
  text-decoration-color: rgba(255, 178, 87, .4);
  text-underline-offset: 3px;
}
@media (max-width: 640px) {
  .nf-wrap { padding: 28px 16px 56px; }
  .nf-card { padding: 32px 18px 28px; }
  .nf-btn { width: 100%; justify-content: center; }
}
</style>
