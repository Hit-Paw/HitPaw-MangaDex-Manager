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
      <p class="nf-code">Error 404</p>
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
  border: 1px solid var(--hp-line-strong);
  border-radius: var(--hp-r);
  background: var(--hp-paper-2);
  padding: 44px 28px 36px;
  outline: 1px solid var(--hp-line);
  outline-offset: 8px;
}
.nf-icon {
  display: inline-block;
  opacity: .95;
}
.nf-code {
  margin: 16px 0 0;
  font-family: var(--hp-font-mono);
  font-size: .72rem;
  font-weight: 700;
  letter-spacing: .3em;
  text-transform: uppercase;
  color: var(--hp-orange-text);
}
.nf-title {
  margin: 10px 0 10px;
  font-size: clamp(1.5rem, 4vw, 1.9rem);
  font-weight: 800;
  letter-spacing: -.03em;
  color: var(--hp-ink);
  border: none;
  padding: 0;
  line-height: 1.15;
}
.nf-title::after {
  content: '';
  display: block;
  width: 44px;
  height: 3px;
  margin: 16px auto 0;
  background: var(--hp-orange);
}
.nf-sub {
  margin: 16px auto 0;
  max-width: 460px;
  color: var(--hp-ink-2);
  line-height: 1.7;
  font-size: .96rem;
}
.nf-sub kbd,
.nf-tip kbd {
  display: inline-block;
  padding: 1px 7px;
  border: 1px solid var(--hp-line);
  border-bottom-width: 2px;
  border-radius: 3px;
  background: var(--hp-paper-3);
  color: var(--hp-ink-2);
  font-family: var(--hp-font-mono);
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
  padding: 12px 18px;
  border: 1px solid var(--hp-line-strong);
  border-radius: 2px;
  text-decoration: none;
  font-family: var(--hp-font-mono);
  font-weight: 700;
  font-size: .78rem;
  letter-spacing: .09em;
  text-transform: uppercase;
  min-height: 44px;
  background: transparent;
  color: var(--hp-ink);
  transition: border-color var(--hp-fast) ease, background var(--hp-fast) ease, color var(--hp-fast) ease;
}
.nf-btn:hover {
  border-color: var(--hp-ink);
  background: var(--hp-wash);
  color: var(--hp-ink);
}
.nf-btn--brand {
  background: var(--hp-orange);
  border-color: var(--hp-orange);
  color: var(--hp-btn-ink);
}
.nf-btn--brand:hover {
  background: var(--hp-orange-deep);
  border-color: var(--hp-orange-deep);
  color: var(--hp-btn-ink);
}
.nf-tip {
  margin: 24px auto 0;
  max-width: 460px;
  color: var(--hp-ink-3);
  font-size: .86rem;
  line-height: 1.7;
  border-top: 1px solid var(--hp-line);
  padding-top: 18px;
}
.nf-tip strong { color: var(--hp-ink-2); }
.nf-tip a {
  color: var(--hp-orange-text);
  text-decoration: underline;
  text-underline-offset: 3px;
}
.nf-tip a:hover { color: var(--hp-orange-head); }
@media (max-width: 640px) {
  .nf-wrap { padding: 28px 16px 56px; }
  .nf-card { padding: 32px 18px 28px; outline-offset: 5px; }
  .nf-btn { width: 100%; justify-content: center; }
}
</style>