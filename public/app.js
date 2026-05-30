function buildUrl() {
  const devid = document.getElementById('devid').value.trim();
  if (!devid) return null;
  const params = new URLSearchParams({
    devid,
    humidityData: document.getElementById('humidity').value,
    celData: document.getElementById('celsius').value,
    fehrData: document.getElementById('fahrenheit').value,
    hicData: document.getElementById('hic').value,
    hifData: document.getElementById('hif').value,
  });
  return `http://api.pushingbox.com/pushingbox?${params.toString()}`;
}

function render() {
  const url = buildUrl();
  const out = document.getElementById('curl-out');
  if (!url) {
    out.textContent = 'Enter your PushingBox device ID first.';
    return;
  }
  out.textContent = `# curl\ncurl "${url}"\n\n# Arduino-style GET path\nGET /pushingbox?${url.split('?')[1]} HTTP/1.1\nHost: api.pushingbox.com`;
}

document.getElementById('generate').addEventListener('click', render);
document.getElementById('send').addEventListener('click', async () => {
  const url = buildUrl();
  const result = document.getElementById('result');
  if (!url) { result.textContent = 'Enter your device ID first.'; return; }
  result.textContent = 'Sending…';
  try {
    await fetch(url, { mode: 'no-cors' });
    result.textContent = 'Request sent (browser may block reading the response — check your Google Sheet).';
  } catch (e) {
    result.textContent = 'Request failed: ' + e.message;
  }
});
render();
