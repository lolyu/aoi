# the swss-common select architecture
* `Select`: use epoll to select I/O ready selectables
    * `m_epoll_fd`: the epoll fd
    * `m_objects`: map to store from selectables' fds to the `Selectable` objects
    * `m_ready`: set to store the I/O ready `Selectable` objects
        * `m_ready` is an ordered queue, as `Selectable` object with higher priority and earlier update time is placed at the front
    * `addSelectable(*selectable)`: add a `Selectable` object
    * `removeSelectable(*selectable)`: remove a `Selectable` object
    * `addSelectables(selectables)`: add multiple `Selectable` objects
    * `select(Selectable **c)`: select an I/O ready selectable
    * `isQueueEmpty()`: check if there is queued I/O ready selectable

* `Selectable`: virtual class to represent a selectable object
    * `m_priority`: the selectable priority
    * `m_last_used_time`: last time of being selected
    * `getFd()`: get the fd
    * `readData()`
    * `hasData()`
    * `hasCachedData()`
    * `initializedWithData()`
    * `updateAfterRead()`: hook to run after every read
    * `getPri()`: return the priority
    * `getLastUsedTime()`: return the last being-selected time
    * `updateLastUsedTime()`: update the last beinng-selected time to now

## more into `Select::select`
### `Select::poll_descriptors`
```cpp
    int Select::poll_descriptors(Selectable **c, unsigned int timeout, bool interrupt_on_signal = false)
    {
        int sz_selectables = static_cast<int>(m_objects.size());
        std::vector<struct epoll_event> events(sz_selectables);
        int ret;

        while (true)
        {
            ret = ::epoll_wait(m_epoll_fd, events.data(), sz_selectables, timeout);
            // on signal interrupt check if we need to return
            if (ret == -1 && errno == EINTR)
                if (interrupt_on_signal)
                    return Select::SIGNALINT;
                // on all other errors break the loop
                else
                    break;
        }

        if (ret < 0)
            return Select::ERROR;

        for (int i = 0; i < ret; ++i)
        {
            int fd = events[i].data.fd;
            Selectable *sel = m_objects[fd];
            try
            {
                sel->readData();
            }
            catch (const std::runtime_error &ex)
            {
                SWSS_LOG_ERROR("readData error: %s", ex.what());
                return Select::ERROR;
            }
            m_ready.insert(sel);
        }

        while (!m_ready.empty())
        {
            auto sel = *m_ready.begin();
            m_ready.erase(sel);
            // we must update clock only when the selector out of the m_ready
            // otherwise we break invariant of the m_ready
            sel->updateLastUsedTime();

            if (!sel->hasData())
                continue;

            *c = sel;

            if (sel->hasCachedData())
                // reinsert Selectable back to the m_ready set, when there're more messages in the cache
                m_ready.insert(sel);

            sel->updateAfterRead();
            return Select::OBJECT;
        }

        return Select::TIMEOUT;
    }
```
* `Select::poll_descriptors`:
    1. poll the registered selectables
    2. for every I/O ready selectable `sel`, call `sel->readData()` and insert it into the I/O ready queue
    3. pops out every `Selectable` object `sel` stored in the I/O ready queue:
        * updates its last update time(`sel->updateLastUsedTime()`)
        * returns the `Selectable` object if it has data to read: `sel->hasData() == true`
        * if it have cached data as `sel->hasCachedData() == false`, put back into the I/O ready queue
    4. if there is a `Selectable` object `sel` returned from step#3, call its after read hook `sel->updateAfterRead()`

### `Select::select`
```cpp
    int Select::select(Selectable **c, int timeout, bool interrupt_on_signal)
    {
        SWSS_LOG_ENTER();
        int ret;
        *c = NULL;
        /* check if we have some data */
        ret = poll_descriptors(c, 0);
        /* return if we have data, we have an error or desired timeout was 0 */
        if (ret != Select::TIMEOUT || timeout == 0)
            return ret;
        /* wait for data */
        ret = poll_descriptors(c, timeout, interrupt_on_signal);
        return ret;
    }
```
